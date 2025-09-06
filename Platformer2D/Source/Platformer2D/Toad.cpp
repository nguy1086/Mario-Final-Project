// Fill out your copyright notice in the Description page of Project Settings.


#include "Toad.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Gib.h"
#include "Blood.h"
#include "MarioCharacter.h"

// Sets default values
AToad::AToad() :
	State(EToadState::Unknown),
	Velocity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("ToadComponent");
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->BodyInstance.bLockXRotation = true;
	BoxComponent->BodyInstance.bLockYRotation = true;
	BoxComponent->BodyInstance.bLockZRotation = true;
	BoxComponent->BodyInstance.bLockYTranslation = true;
	RootComponent = BoxComponent;

	CheckOverlap = CreateDefaultSubobject<UBoxComponent>("CheckOverlap");
	CheckOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CheckOverlap->SetCollisionProfileName("OverlapAll");
	CheckOverlap->SetGenerateOverlapEvents(true);
	CheckOverlap->OnComponentBeginOverlap.AddDynamic(this, &AToad::OnCheckOverlap);
	CheckOverlap->SetupAttachment(RootComponent);

	// Create the Flipbook component and attach it to the root component
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("ToadFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	// Add "Enemy" to the Actor tags
	Tags.Add("Enemy");
}

// Called when the game starts or when spawned
void AToad::BeginPlay()
{
	Super::BeginPlay();
	SetState(EToadState::Idle);
}

void AToad::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Bullet"))
	{
		SetState(EToadState::Shotted);
		OtherActor->Destroy();
	}
	else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
	{
		SetState(EToadState::Exploded);
	}
}

void AToad::OnCheckOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Bullet"))
	{
		SetState(EToadState::Shotted);
		OtherActor->Destroy();
	}
	else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
	{
		SetState(EToadState::Exploded);
	}
}

// Called every frame
void AToad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EToadState::Idle)
	{
		AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();

		FVector marioloc = mario->GetActorLocation();

		if (marioloc.X >= GetActorLocation().X - 256.0f)
		{
			SetState(EToadState::Jumping);
		}
	}
	else if (State == EToadState::Shotted)
	{
		FVector location = GetActorLocation();
		location.Z += Velocity.Z * DeltaTime;
		SetActorLocation(location);
		FlipbookComponent->SetRelativeRotation(FRotator(Velocity.Z, 0.0f, 0.0f));
		if (GetActorLocation().Z < 0.0f)
		{
			Destroy();
		}
		else
		{
			Velocity.Z -= 550.0f * DeltaTime;
		}
	}
}

void AToad::SetState(EToadState state)
{
	if (State != state)
	{
		State = state;

		if (State == EToadState::Idle)
		{
			BoxComponent->SetSimulatePhysics(true);

		}
		else if (State == EToadState::Jumping)
		{
			BoxComponent->SetSimulatePhysics(false);
		}
		else if (State == EToadState::Shotted)
		{
			BoxComponent->SetSimulatePhysics(true);
			Velocity.Z = -150.0f;
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			FlipbookComponent->SetCollisionProfileName("NoCollision");
		}
		else if (State == EToadState::Exploded)
		{
			FVector location = GetActorLocation();
			const int32 kNumBits = 3;
			AGib* bits[kNumBits] = {};
			FVector locations[kNumBits] = { location + FVector(-8.0f, 0.0f, 8.0f),
											location + FVector(-8.0f, 0.0f, -8.0f),
											location + FVector(8.0f, 0.0f, 8.0f) };

			FVector impulses[kNumBits] = { FVector(-150.0f, 0.0f, 350.0f),
											FVector(-150.0f, 0.0f, 300.0f),
											FVector(150.0f, 0.0f, 350.0f) };

			EGibType types[kNumBits] = { EGibType::ToadHead,
														EGibType::ToadArm,
														EGibType::ToadArm };

			for (int32 i = 0; i < kNumBits; i++)
			{
				bits[i] = GetWorld()->SpawnActor<AGib>(GibTemplate, locations[i], FRotator(0.0f, 0.0f, 0.0f));
				bits[i]->Spawn(types[i], impulses[i]);
			}
			ABlood* blood = GetWorld()->SpawnActor<ABlood>(BloodTemplate, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
			blood->Spawn(false);

			Destroy();
		}

		UpdateFlipbook();
	}
}

void AToad::UpdateFlipbook()
{
	UPaperFlipbook* flipbook = nullptr;

	if (State == EToadState::Idle || State == EToadState::Shotted)
	{
		flipbook = IdleFlipbook;
		FlipbookComponent->SetRelativeLocation(FVector(0, 0, 0.0f));
	}
	else if (State == EToadState::Jumping)
	{
		flipbook = JumpingFlipbook;
		FlipbookComponent->SetRelativeLocation(FVector(0, 0, -6.0f));
	}

	FlipbookComponent->SetFlipbook(flipbook);
	FlipbookComponent->PlayFromStart();
}


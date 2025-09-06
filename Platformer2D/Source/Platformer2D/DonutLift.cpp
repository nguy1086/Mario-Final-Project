// Fill out your copyright notice in the Description page of Project Settings.


#include "DonutLift.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "GameConstants.h"
#include "MarioCharacter.h"
#include "MarioController.h"

// Sets default values
ADonutLift::ADonutLift() :
	Active(0.0f),
	ActiveFlipbook(nullptr),
	BreakingFlipbook(nullptr),
	Velocity(),
	ShakeX(0.0f),
	TopOverlap(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the BoxComponent and set it as the Root component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("DonutBoxComponent");
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->OnComponentHit.AddDynamic(this, &ADonutLift::OnHit);
	RootComponent = BoxComponent;

	TopOverlap = CreateDefaultSubobject<UBoxComponent>("DonutBackOverlap");
	TopOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TopOverlap->SetCollisionProfileName("OverlapAll");
	TopOverlap->SetGenerateOverlapEvents(true);
	TopOverlap->OnComponentBeginOverlap.AddDynamic(this, &ADonutLift::OnTopOverlap);
	TopOverlap->SetupAttachment(RootComponent);

	// Create the FlipbookComponent and attach it to the Root component
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("DonutFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);
	
	Tags.Add("World");
}

// Called when the game starts or when spawned
void ADonutLift::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ADonutLift::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (State == EDonutState::Active)
	{

	}
	else if (State == EDonutState::Breaking)
	{
		FVector locDonut = GetActorLocation();
		AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();
		FVector marioLocation = mario->GetActorLocation();

		bool checkY = FMath::Clamp(marioLocation.X, locDonut.X - 8.0f, locDonut.X + 8.0f) == marioLocation.X;
		if (/*checkY && */ FMath::Abs(locDonut.Z - marioLocation.Z) > 50.0f ||
			FMath::Abs(locDonut.X - marioLocation.X) > 32.0f)
		{
			SetState(EDonutState::Active);
			FlipbookComponent->SetRelativeLocation(FVector(0, 0, 0));
		}
		else
		{
			Active -= DeltaTime;

			FVector loc = FlipbookComponent->GetRelativeLocation();
			ShakeX = FMath::RandRange(GameConstants::DonutShakeXMin, GameConstants::DonutShakeXMax);
			loc.X = ShakeX;
			FlipbookComponent->SetRelativeLocation(loc);

			if (Active < 0.0f)
			{
				Active = 0.0f;
				SetState(EDonutState::Dropping);
				FlipbookComponent->SetRelativeLocation(FVector(0, 0, 0));
			}
		}
	}
	else if (State == EDonutState::Dropping)
	{
		FVector loc = GetActorLocation();
		loc.Z += Velocity.Z * DeltaTime;
		SetActorLocation(loc);
		if (loc.Z < 0.0f)
		{
			Destroy();
		}
	}
}

void ADonutLift::SetState(EDonutState state)
{
	if (State != state)
	{
		State = state;
		if (State == EDonutState::Active)
		{
			Active = GameConstants::DonutTimer;
		}
		else if (State == EDonutState::Breaking)
		{
			Active = GameConstants::DonutTimer;
		}
		else if (State == EDonutState::Dropping)
		{
			Velocity.Z = -GameConstants::DonutDropSpeed;
		}
	}
	UpdateFlipbook();
}

void ADonutLift::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if (OtherActor->ActorHasTag("MarioCharacter") && State != EDonutState::Dropping)
	//{
	//	SetState(EDonutState::Breaking);
	//}
	//else if (State != EDonutState::Dropping)
	//{
	//	SetState(EDonutState::Active);
	//}
}

void ADonutLift::OnTopOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("MarioCharacter") && State != EDonutState::Dropping)
	{
		SetState(EDonutState::Breaking);
	}
	else if (State != EDonutState::Dropping)
	{
		SetState(EDonutState::Active);
	}
}

void ADonutLift::UpdateFlipbook()
{
    UPaperFlipbook* flipbook = nullptr;

    if (State == EDonutState::Active)
    {
        flipbook = ActiveFlipbook;
    }
    else if (State == EDonutState::Breaking ||
        State == EDonutState::Dropping)
    {
        flipbook = BreakingFlipbook;
    }

    FlipbookComponent->SetFlipbook(flipbook);
    FlipbookComponent->PlayFromStart();
}


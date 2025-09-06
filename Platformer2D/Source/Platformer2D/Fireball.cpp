// Fill out your copyright notice in the Description page of Project Settings.


#include "Fireball.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "MarioCharacter.h"
#include "MarioPlayerState.h"
#include "GameConstants.h"
// Sets default values
AFireball::AFireball() :
	Velocity(),
	CheckOverlap(nullptr),
	LifeTime(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("FireballComponent");
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName("Fireball");
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->OnComponentHit.AddDynamic(this, &AFireball::OnHit);
	BoxComponent->BodyInstance.bLockXRotation = true;
	BoxComponent->BodyInstance.bLockYRotation = true;
	BoxComponent->BodyInstance.bLockZRotation = true;
	BoxComponent->BodyInstance.bLockYTranslation = true;
	RootComponent = BoxComponent;

	CheckOverlap = CreateDefaultSubobject<UBoxComponent>("BulletBillCheckOverlap");
	CheckOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CheckOverlap->SetCollisionProfileName("OverlapAll");
	CheckOverlap->SetGenerateOverlapEvents(true);
	CheckOverlap->OnComponentBeginOverlap.AddDynamic(this, &AFireball::OnCheckOverlap);
	CheckOverlap->SetupAttachment(RootComponent);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("DiscFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	Tags.Add("Fireball");
}

void AFireball::Spawn()
{
	AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();
	PlayerState = mario->GetPlayerState<AMarioPlayerState>();
	EMarioDirection marioDirection = PlayerState->Direction;
	FVector marioloc = mario->GetActorLocation();

	if (marioDirection == EMarioDirection::Left)
	{
		Velocity.X = -GameConstants::FireballSpeed;
		SetActorLocation(FVector(marioloc.X - 24.0f, 0.0f, marioloc.Z + 8.0f));
	}
	
	else if (marioDirection == EMarioDirection::Right)
	{
		Velocity.X = GameConstants::FireballSpeed;
		SetActorLocation(FVector(marioloc.X + 24.0f, 0.0f, marioloc.Z + 8.0f));
	}
}

// Called when the game starts or when spawned
void AFireball::BeginPlay()
{
	Super::BeginPlay();
	LifeTime = GameConstants::FireballLifeTime;
}

// Called every frame
void AFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector location = GetActorLocation();
	location += Velocity * DeltaTime;
	SetActorLocation(location);

	if (Velocity.Z < 0.0f)
	{
		Velocity.Z = 0.0f;
	}
	else
	{
		Velocity.Z -= 1 * 2050.0f * DeltaTime;
	}

	LifeTime -= DeltaTime;
	if (LifeTime < 0.0f)
	{
		Destroy();
	}
}

void AFireball::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		if (OtherActor->ActorHasTag("Thwomp"))
		{
			if (Velocity.X < 0.0f)
				Velocity.X = GameConstants::FireballSpeed;
			else if (Velocity.X > 0.0f)
				Velocity.X = -GameConstants::FireballSpeed;
		}
		else
		{
			LifeTime = 0.001f;
		}
	}
	else if (OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("ItemBox") || OtherActor->ActorHasTag("BreakableBlock"))
	{
		if	(Hit.Normal.X <= -0.8f || Hit.Normal.X >= 0.8f)
		{
			if (Velocity.X < 0.0f)
				Velocity.X = GameConstants::FireballSpeed;
			else if (Velocity.X > 0.0f)
				Velocity.X = -GameConstants::FireballSpeed;
		}
		else if (Hit.Normal.Z >= 0.7f)
		{
			Velocity.Z = GameConstants::FireballSpeed;
		}

	}
}

void AFireball::OnCheckOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		if (OtherActor->ActorHasTag("Thwomp"))
		{
			if (Velocity.X < 0.0f)
				Velocity.X = GameConstants::FireballSpeed;
			else if (Velocity.X > 0.0f)
				Velocity.X = -GameConstants::FireballSpeed;
		}
		else
		{
			LifeTime = 0.01f;
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Bobomb.h"
#include "EnemyConstants.h"
#include "MarioCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Explosion.h"

ABobomb::ABobomb() :
	InitialDirection(EBobombInitialDirection::Random),
	FrontOverlap(nullptr),
	BackOverlap(nullptr),
	WalkingFlipbook(nullptr),
	StunnedFlipbook(nullptr),
	Velocity(),
	StunnedDuration(0.0f),
	State(EBobombState::Unknown)
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->OnComponentHit.AddDynamic(this, &ABobomb::OnHit);

	FrontOverlap = CreateDefaultSubobject<UBoxComponent>("GoombaFrontOverlap");
	FrontOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FrontOverlap->SetCollisionProfileName("OverlapAll");
	FrontOverlap->SetGenerateOverlapEvents(true);
	FrontOverlap->OnComponentBeginOverlap.AddDynamic(this, &ABobomb::OnForwardOverlap);
	FrontOverlap->SetupAttachment(RootComponent);

	BackOverlap = CreateDefaultSubobject<UBoxComponent>("GoombaEnemyBackOverlap");
	BackOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BackOverlap->SetCollisionProfileName("OverlapAll");
	BackOverlap->SetGenerateOverlapEvents(true);
	BackOverlap->OnComponentBeginOverlap.AddDynamic(this, &ABobomb::OnBackwardOverlap);
	BackOverlap->SetupAttachment(RootComponent);

	Type = EEnemyType::Bobomb;
	Tags.Add("Bobomb");
}

void ABobomb::BeginPlay()
{
	Super::BeginPlay();

	SetState(EBobombState::Walking);
}

void ABobomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EBobombState::Walking)
	{
		FVector location = GetActorLocation();
		location += Velocity * DeltaTime;
		SetActorLocation(location);
	}
	else if (State == EBobombState::Stunned)
	{
		if (StunnedDuration > 0.0f)
		{
			StunnedDuration -= DeltaTime;

			if (StunnedDuration < 0.0f)
			{
				StunnedDuration = 0.0f;
				SetState(EBobombState::Exploded);
			}
		}
	}
}

void ABobomb::SetState(EBobombState state)
{
	if (State != state)
	{
		State = state;

		if (State == EBobombState::Walking)
		{
			ApplyInitialVelocity();
			BoxComponent->SetCollisionProfileName("EnemyDynamic");
		}
		else if (State == EBobombState::Stunned)
		{
			StunnedDuration = EnemyConstants::BobombStunnedDuration;
			BoxComponent->SetCollisionProfileName("BlockAllDynamic");
		}
		else if (State == EBobombState::Exploded)
		{
			AExplosion* explosion = GetWorld()->SpawnActor<AExplosion>(ExplosionTemplate, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
			explosion->Spawn();

			Destroy();
		}
		UpdateFlipbook();
	}
}

void ABobomb::OnForwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("Goomba") || OtherActor->ActorHasTag("MushroomPickup") || OtherActor->ActorHasTag("DryBones") || OtherActor->ActorHasTag("BoomBoom") || OtherActor->ActorHasTag("Bobomb"))
	{
		Velocity.X = -EnemyConstants::BobombSpeed;
	}
	else if (OtherActor->ActorHasTag("Bullet") || OtherActor->ActorHasTag("Fireball"))
	{
		SetState(EBobombState::Exploded);
		OtherActor->Destroy();
	}
	else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
	{
		SetState(EBobombState::Exploded);
	}
	UpdateFlipbook();
}

void ABobomb::OnBackwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("Goomba") || OtherActor->ActorHasTag("MushroomPickup") || OtherActor->ActorHasTag("DryBones") || OtherActor->ActorHasTag("BoomBoom") || OtherActor->ActorHasTag("Bobomb"))
	{
		Velocity.X = EnemyConstants::BobombSpeed;
	}
	else if (OtherActor->ActorHasTag("Bullet") || OtherActor->ActorHasTag("Fireball"))
	{
		SetState(EBobombState::Exploded);
		OtherActor->Destroy();
	}
	else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
	{
		SetState(EBobombState::Exploded);
	}
	UpdateFlipbook();
}

void ABobomb::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		if ((Hit.Normal.X <= -0.8f || Hit.Normal.X >= 0.8f) && State == EBobombState::Walking)
		{
			Cast<AMarioCharacter>(OtherActor)->HandleDamage();
		}
		else if (Hit.Normal.Z >= 0.7f && State == EBobombState::Walking)
		{
			Cast<AMarioCharacter>(OtherActor)->HandleDamage();
		}
		else if (Hit.Normal.Z <= -0.7f)
		{
			// Mario landed on Goomba
			if (State == EBobombState::Walking)
			{
				SetState(EBobombState::Stunned);
				Cast<AMarioCharacter>(OtherActor)->ApplyBounce();
			}
		}
	}
	else if (OtherActor->ActorHasTag("Bullet") || OtherActor->ActorHasTag("Fireball"))
	{
		SetState(EBobombState::Exploded);
		OtherActor->Destroy();
	}
	else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
	{
		SetState(EBobombState::Exploded);
	}
}

void ABobomb::ApplyInitialVelocity()
{
	if (InitialDirection == EBobombInitialDirection::Random)
	{
		int32 index = FMath::RandRange(0, 9) % 2;
		float speeds[] = { EnemyConstants::BobombSpeed, -EnemyConstants::BobombSpeed };
		Velocity.X = speeds[index];
	}
	else if (InitialDirection == EBobombInitialDirection::Right)
	{
		Velocity.X = EnemyConstants::BobombSpeed;
	}
	else if (InitialDirection == EBobombInitialDirection::Left)
	{
		Velocity.X = -EnemyConstants::BobombSpeed;
	}
}

void ABobomb::UpdateFlipbook()
{
	UPaperFlipbook* flipbook = nullptr;

	if (State == EBobombState::Walking)
	{
		flipbook = WalkingFlipbook;

		if (Velocity.X > 0.0f)
			FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		else if (Velocity.X < 0.0f)
			FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	else if (State == EBobombState::Stunned)
	{
		flipbook = StunnedFlipbook;
	}

	FlipbookComponent->SetFlipbook(flipbook);
	FlipbookComponent->PlayFromStart();
}

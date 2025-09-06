// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike.h"
#include "EnemyConstants.h"
#include "MarioCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"
#include "MarioCharacter.h"
#include "SpikeBall.h"
#include "GameConstants.h"

ASpike::ASpike() :
	InitialDirection(ESpikeInitialDirection::Random),
	FrontOverlap(nullptr),
	BackOverlap(nullptr),
	WalkingFlipbook(nullptr),
	SquishedFlipbook(nullptr),
	Velocity(),
	SquishedDuration(0.0f),
	WalkingDuration(0.0f),
	ReloadingDuration(0.0f),
	AimmingDuration(0.0f),
	State(ESpikeState::Unknown)
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->OnComponentHit.AddDynamic(this, &ASpike::OnHit);

	FrontOverlap = CreateDefaultSubobject<UBoxComponent>("SpikeFrontOverlap");
	FrontOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FrontOverlap->SetCollisionProfileName("OverlapAll");
	FrontOverlap->SetGenerateOverlapEvents(true);
	FrontOverlap->OnComponentBeginOverlap.AddDynamic(this, &ASpike::OnForwardOverlap);
	FrontOverlap->SetupAttachment(RootComponent);

	BackOverlap = CreateDefaultSubobject<UBoxComponent>("SpikeEnemyBackOverlap");
	BackOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BackOverlap->SetCollisionProfileName("OverlapAll");
	BackOverlap->SetGenerateOverlapEvents(true);
	BackOverlap->OnComponentBeginOverlap.AddDynamic(this, &ASpike::OnBackwardOverlap);
	BackOverlap->SetupAttachment(RootComponent);

	Type = EEnemyType::Spike;
	Tags.Add("Spike");
}

void ASpike::BeginPlay()
{
	Super::BeginPlay();

	SetState(ESpikeState::Walking);
}

void ASpike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == ESpikeState::Walking)
	{
		FVector location = GetActorLocation();
		location += Velocity * DeltaTime;
		SetActorLocation(location);

		WalkingDuration -= DeltaTime;

		AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();

		if (WalkingDuration < 0.0f)
		{
			//only shoot on/near screen
			if (GetActorLocation().X > mario->GetActorLocation().X - (GameConstants::ResolutionWidth) &&
				GetActorLocation().X < mario->GetActorLocation().X + (GameConstants::ResolutionWidth))
			{
				FVector marioLoc = mario->GetActorLocation();
				FVector spikeLoc = GetActorLocation();
				if (marioLoc.X < spikeLoc.X)
					Velocity.X = -EnemyConstants::GoombaSpeed;
				else if (marioLoc.X > spikeLoc.X)
				{
					Velocity.X = EnemyConstants::GoombaSpeed;
				}

				WalkingDuration = 0.0f;
				SetState(ESpikeState::Reloading);
			}
			else
			{
				SetState(ESpikeState::Walking);
			}
		}
	}
	else if (State == ESpikeState::Reloading)
	{
		ReloadingDuration -= DeltaTime;

		if (ReloadingDuration < 0.0f)
		{
			ReloadingDuration = 0.0f;
			SetState(ESpikeState::Aimming);
		}
	}
	else if (State == ESpikeState::Aimming)
	{
		AimmingDuration -= DeltaTime;

		if (AimmingDuration < 0.0f)
		{
			AimmingDuration = 0.0f;
			SetState(ESpikeState::Walking);
		}
	}
	else if (State == ESpikeState::Squished)
	{
		if (SquishedDuration > 0.0f)
		{
			SquishedDuration -= DeltaTime;

			if (SquishedDuration < 0.0f)
			{
				SquishedDuration = 0.0f;
				Destroy();
			}
		}
	}
}

void ASpike::SetState(ESpikeState state)
{
	if (State != state)
	{
		State = state;

		if (State == ESpikeState::Walking)
		{
			ApplyInitialVelocity();
			BoxComponent->SetCollisionProfileName("EnemyDynamic");
			WalkingDuration = FMath::RandRange(EnemyConstants::SpikeToReloadMin, EnemyConstants::SpikeToReloadMax);
		}
		else if (State == ESpikeState::Reloading)
		{
			ReloadingDuration = EnemyConstants::SpikeReloading;
			ASpikeBall* spikeball = GetWorld()->SpawnActor<ASpikeBall>(SpikeBallTemplate, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
			spikeball->Spawn(Velocity.X);
		}
		else if (State == ESpikeState::Aimming)
		{
			AimmingDuration = EnemyConstants::SpikeAimming;
		}
		else if (State == ESpikeState::Squished)
		{
			SquishedDuration = EnemyConstants::GoombaSquishedDuration;
			BoxComponent->SetCollisionProfileName("EnemyDead");
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else if (State == ESpikeState::Exploded)
		{
			Destroy();
		}
		UpdateFlipbook();
	}
}

void ASpike::OnForwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("Goomba") || OtherActor->ActorHasTag("MushroomPickup") || OtherActor->ActorHasTag("DryBones") || OtherActor->ActorHasTag("BoomBoom") || OtherActor->ActorHasTag("Bobomb"))
	{
		Velocity.X = -EnemyConstants::GoombaSpeed;
	}
	else if (OtherActor->ActorHasTag("Bullet") || OtherActor->ActorHasTag("Fireball"))
	{
		SetState(ESpikeState::Squished);
		OtherActor->Destroy();
	}
	else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
	{
		SetState(ESpikeState::Exploded);
	}
	UpdateFlipbook();
}

void ASpike::OnBackwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("Goomba") || OtherActor->ActorHasTag("MushroomPickup") || OtherActor->ActorHasTag("DryBones") || OtherActor->ActorHasTag("BoomBoom") || OtherActor->ActorHasTag("Bobomb"))
	{
		Velocity.X = EnemyConstants::GoombaSpeed;
	}
	else if (OtherActor->ActorHasTag("Bullet") || OtherActor->ActorHasTag("Fireball"))
	{
		SetState(ESpikeState::Squished);
		OtherActor->Destroy();
	}
	else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
	{
		SetState(ESpikeState::Exploded);
	}
	UpdateFlipbook();
}

void ASpike::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		if (Hit.Normal.X <= -0.8f || Hit.Normal.X >= 0.8f)
		{
			// Goomba hit Mario on the side
			Cast<AMarioCharacter>(OtherActor)->HandleDamage();
		}
		else if (Hit.Normal.Z >= 0.7f)
		{
			// Goomba landed on Mario
			Cast<AMarioCharacter>(OtherActor)->HandleDamage();
		}
		else if (Hit.Normal.Z <= -0.7f)
		{
			// Mario landed on Goomba
			if (State == ESpikeState::Walking)
			{
				SetState(ESpikeState::Squished);
				Cast<AMarioCharacter>(OtherActor)->ApplyBounce();
			}
		}
	}
	else if (OtherActor->ActorHasTag("Bullet") || OtherActor->ActorHasTag("Fireball"))
	{
		SetState(ESpikeState::Squished);
		OtherActor->Destroy();
	}
	else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
	{
		SetState(ESpikeState::Exploded);
	}
}

void ASpike::ApplyInitialVelocity()
{
	if (Velocity == FVector(NULL, NULL, NULL))//moves at same direction no matter
	{
		if (InitialDirection == ESpikeInitialDirection::Random)
		{
			int32 index = FMath::RandRange(0, 9) % 2;
			float speeds[] = { EnemyConstants::GoombaSpeed, -EnemyConstants::GoombaSpeed };
			Velocity.X = speeds[index];
		}
		else if (InitialDirection == ESpikeInitialDirection::Right)
		{
			Velocity.X = EnemyConstants::GoombaSpeed;
		}
		else if (InitialDirection == ESpikeInitialDirection::Left)
		{
			Velocity.X = -EnemyConstants::GoombaSpeed;
		}
	}
}

void ASpike::UpdateFlipbook()
{
	UPaperFlipbook* flipbook = nullptr;

	if (State == ESpikeState::Walking)
	{
		flipbook = WalkingFlipbook;

		if (Velocity.X > 0.0f)
			FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		else if (Velocity.X < 0.0f)
			FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	else if (State == ESpikeState::Reloading)
	{
		flipbook = ReloadingFlipbook;
		if (Velocity.X > 0.0f)
			FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		else if (Velocity.X < 0.0f)
			FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	else if (State == ESpikeState::Aimming)
	{
		flipbook = AimmingFlipbook;
	}
	else if (State == ESpikeState::Squished)
	{
		flipbook = SquishedFlipbook;
	}

	FlipbookComponent->SetFlipbook(flipbook);
	FlipbookComponent->PlayFromStart();
}
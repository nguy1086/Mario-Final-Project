// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "GameConstants.h"
#include "PickupConstants.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "PlatformerGameModeBase.h"

// Sets default values
APickup::APickup()  :
	BoxComponent(nullptr),
	FlipbookComponent(nullptr),
	Type(EPickupType::Unknown),
	State(EPickupState::Unknown),
	Start(),
	Target(),
	SpawnTime(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the Sphere component and set it as the root component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("PickupBoxComponent");
	BoxComponent->SetCollisionProfileName("OverlapAll");
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->BodyInstance.bLockXRotation = true;
	BoxComponent->BodyInstance.bLockYRotation = true;
	BoxComponent->BodyInstance.bLockZRotation = true;
	BoxComponent->BodyInstance.bLockYTranslation = true;
	RootComponent = BoxComponent;

	// Create the Flipbook component and attach it to the root component
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("PickupFlipbookComponent");
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	// Add "Pickup" to the Actor tags
	Tags.Add("Pickup");
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EPickupState::Spawning)
	{
		if (SpawnTime > 0.0f)
		{
			SpawnTime -= DeltaTime;

			float pct = 1.0f - (SpawnTime / PickupConstants::PickupAnimationDuration);
			FVector location = FMath::Lerp(Start, Target, pct);
			SetActorLocation(location);

			if (SpawnTime <= 0.0f)
			{
				SpawnTime = 0.0f;
				SetActorLocation(Target);
				State = EPickupState::Active;
				OnPickupActive();
			}
		}
	}
	else if (State == EPickupState::Active && Type != EPickupType::Coin)
	{
		APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
		EGameState state = gameMode->State;
		if (state == EGameState::FadeOut)
		{
			Destroy();
		}
	}
}

EPickupType APickup::GetPickupType()
{
	return Type;
}

void APickup::Spawn()
{
	State = EPickupState::Spawning;
	SpawnTime = PickupConstants::PickupAnimationDuration;
	Start = GetActorLocation();
	Target = Start;
	Target.Z += GameConstants::TileSize;
}

void APickup::OnPickupActive()
{
	// Override this method in derived classes, if needed
}
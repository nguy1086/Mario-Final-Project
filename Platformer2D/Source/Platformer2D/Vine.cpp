// Fill out your copyright notice in the Description page of Project Settings.


#include "Vine.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "PickupConstants.h"
#include "GameConstants.h"
#include "VineExtended.h"
#include "PlatformerGameModeBase.h"
#include "Door.h"

// Sets default values
AVine::AVine() :
	State(EVineState::Unknown),
	Target(),
	Start(),
	SpawnTime(0.0f),
	ZIncrement(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("VineBoxComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionProfileName("OverlapAll");
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetSimulatePhysics(false);
	RootComponent = BoxComponent;

	// Create the FlipbookComponent and attach it to the Root component
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("VineFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	Tags.Add("Vine");
}

// Called when the game starts or when spawned
void AVine::BeginPlay()
{
	Super::BeginPlay();
	Original = GetActorLocation();
}

// Called every frame
void AVine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EVineState::Spawning)
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
				State = EVineState::Active;
				SpawnTime = PickupConstants::PickupAnimationDuration;
			}
		}
	}
	else if (State == EVineState::Active)
	{
		if ((ZIncrement < 256.0f &&  GetActorLocation().Z < Original.Z + 256.0f)&& 
			SpawnTime > 0.0f)
		{
			SpawnTime -= DeltaTime;

			if (SpawnTime < 0.0f)
			{
				FVector location = GetActorLocation();
				location.Z += ZIncrement;
				ZIncrement += (GameConstants::TileSize - 2.0f);
				AVineExtended* vine = GetWorld()->SpawnActor<AVineExtended>(VineExtendedTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
				vine->Spawn();

				SpawnTime = PickupConstants::PickupAnimationDuration;
			}
		}
		APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
		EGameState state = gameMode->State;
		if (state == EGameState::FadeOut)
		{
			Destroy();
		}
	}
}

void AVine::Spawn()
{
	State = EVineState::Spawning;
	SpawnTime = PickupConstants::PickupAnimationDuration;
	Start = GetActorLocation();
	Target = Start;
	Target.Z += (GameConstants::TileSize);
}

void AVine::DoorAction()
{
	APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
	if (gameMode != nullptr)
	{
		gameMode->NextRoom(FVector(5232.0, 0.0f, 208.0));
	}
}
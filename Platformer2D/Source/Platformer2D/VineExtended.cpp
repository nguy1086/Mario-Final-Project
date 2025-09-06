// Fill out your copyright notice in the Description page of Project Settings.


#include "VineExtended.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "PickupConstants.h"
#include "GameConstants.h"
#include "PlatformerGameModeBase.h"

// Sets default values
AVineExtended::AVineExtended()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("VineBoxComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionProfileName("OverlapAll");
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetSimulatePhysics(false);
	//BoxComponent->OnComponentHit.AddDynamic(this, &AItemBox::OnHit);
	RootComponent = BoxComponent;

	// Create the FlipbookComponent and attach it to the Root component
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("VineFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AVineExtended::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVineExtended::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EVineExtendedState::Spawning)
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
				State = EVineExtendedState::Active;
			}
		}
	}
	else if (State == EVineExtendedState::Active)
	{
		APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
		EGameState state = gameMode->State;
		if (state == EGameState::FadeOut)
		{
			Destroy();
		}
	}
}

void AVineExtended::Spawn()
{
	State = EVineExtendedState::Spawning;
	SpawnTime = PickupConstants::PickupAnimationDuration;
	Start = GetActorLocation();
	Target = Start;
	Target.Z += GameConstants::TileSize;
}
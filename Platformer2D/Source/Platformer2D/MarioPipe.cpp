// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioPipe.h"
#include "Door.h"
#include "PlatformerGameModeBase.h"
#include "Components/BoxComponent.h"
#include "GameConstants.h"
#include "PickupConstants.h"
#include "MarioCharacter.h"
// Sets default values
AMarioPipe::AMarioPipe() :
	State(EWarpState::Unknown),
	AnimationTimer(0.0f),
	Start(),
	Target(),
	Mario(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PipeBoxComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionProfileName("OverlapAll");
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetSimulatePhysics(false);

	RootComponent = BoxComponent;

	Tags.Add("Pipe");
}

void AMarioPipe::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMarioPipe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EWarpState::Warping)
	{
		AnimationTimer -= 0.25f * DeltaTime;

		float pct = 1.0f - (AnimationTimer / PickupConstants::ItemBoxAnimationDuration);
		FVector location = FMath::Lerp(Start, Target, pct);
		location.Y = -1.0f;
		Mario->SetActorLocation(location);

		if (AnimationTimer < 0.0f)
		{
			State = EWarpState::Unknown;
			location.Y = 0.0f;
			Mario->SetActorLocation(location);
		}
	}

}

void AMarioPipe::DoorAction()
{
	APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
	if (gameMode != nullptr)
	{
		Mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();
		AnimationTimer = (PickupConstants::ItemBoxAnimationDuration);
		Start = Mario->GetActorLocation();
		Target = Start;
		Target.Z -= (GameConstants::TileSize * 1.9);
		gameMode->NextRoom(Counterpart);
		State = EWarpState::Warping;
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "PiranhaPlant.h"
#include "EnemyConstants.h"
#include "MarioCharacter.h"
#include "MarioController.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h" 


APiranhaPlant::APiranhaPlant() :
	State(EPiranhaPlantState::Unknown),
	HidingLocation(),
	ActiveLocation(),
	HoldTimer(0.0f),
	AnimationTimer(0.0f)
{
	Type = EEnemyType::PiranhaPlant;
	Tags.Add("PiranhaPlant");
}

void APiranhaPlant::BeginPlay()
{
	Super::BeginPlay();

	// Capture the starting location (hiding) and from that calculate the active (on-screen) location
	HidingLocation = GetActorLocation();
	ActiveLocation = HidingLocation + FVector(0.0f, -0.0f, EnemyConstants::PiranhaPlantOffsetZ);

	// Set the state to hiding
	SetState(EPiranhaPlantState::Hiding);
}

void APiranhaPlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EPiranhaPlantState::Hiding)
	{
		HoldTimer -= DeltaTime;

		if (HoldTimer <= 0.0f)
		{
			HoldTimer = 0.0f;
			SetState(EPiranhaPlantState::AnimatingUp);
		}
	}
	else if (State == EPiranhaPlantState::AnimatingUp)
	{
		AnimationTimer -= DeltaTime;

		float pct = 1.0f - (AnimationTimer / EnemyConstants::PiranhaPlantAnimationDuration);
		FVector location = FMath::Lerp(HidingLocation, ActiveLocation, pct);
		SetActorLocation(location);

		if (AnimationTimer <= 0.0f)
		{
			AnimationTimer = 0.0f;
			SetState(EPiranhaPlantState::Active);
		}
	}
	else if (State == EPiranhaPlantState::Active)
	{
		HoldTimer -= DeltaTime;

		if (HoldTimer <= 0.0f)
		{
			HoldTimer = 0.0f;
			SetState(EPiranhaPlantState::AnimatingDown);
		}
	}
	else if (State == EPiranhaPlantState::AnimatingDown)
	{
		AnimationTimer -= DeltaTime;

		float pct = 1.0f - (AnimationTimer / EnemyConstants::PiranhaPlantAnimationDuration);
		FVector location = FMath::Lerp(ActiveLocation, HidingLocation, pct);
		SetActorLocation(location);

		if (AnimationTimer <= 0.0f)
		{
			AnimationTimer = 0.0f;
			SetState(EPiranhaPlantState::Hiding);
		}
	}
}

void APiranhaPlant::SetState(EPiranhaPlantState state)
{
	if (State != state)
	{
		State = state;

		if (State == EPiranhaPlantState::Hiding)
		{
			SetActorLocation(HidingLocation);
			HoldTimer = FMath::RandRange(EnemyConstants::PiranhaPlantHiddenDurationMin, EnemyConstants::PiranhaPlantHiddenDurationMax);
		}
		else if (State == EPiranhaPlantState::AnimatingUp)
		{
			AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();
			FVector marioLocation = mario->GetActorLocation();

			// Mario is on top of the pipe, don't spawn the piranha plant
			bool checkY = FMath::Clamp(marioLocation.X, ActiveLocation.X - 32.0f, ActiveLocation.X + 32.0f) == marioLocation.X;
			if (checkY && FMath::Abs(ActiveLocation.Z - marioLocation.Z) < 16.5f)
			{
				SetState(EPiranhaPlantState::Hiding);
				return;
			}

			AnimationTimer = EnemyConstants::PiranhaPlantAnimationDuration;
		}
		else if (State == EPiranhaPlantState::Active)
		{
			SetActorLocation(ActiveLocation);
			HoldTimer = EnemyConstants::PiranhaPlantActiveDuration;
		}
		else if (State == EPiranhaPlantState::AnimatingDown)
		{
			AnimationTimer = EnemyConstants::PiranhaPlantAnimationDuration;
		}
	}
}
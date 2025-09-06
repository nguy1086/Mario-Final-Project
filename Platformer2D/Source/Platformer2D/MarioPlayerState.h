// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MarioPlayerState.generated.h"


UENUM(BlueprintType)
enum class EMarioState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Walking		UMETA(DisplayName = "Walking"),
	Jumping		UMETA(DisplayName = "Jumping"),
	Falling		UMETA(DisplayName = "Falling"),
	Ducking		UMETA(DisplayName = "Ducking"),
	Dead		UMETA(DisplayName = "Dead"),
	Gliding		UMETA(DisplayName = "Gliding"),
	Flying		UMETA(DisplayName = "FLying")
};

UENUM(BlueprintType)
enum class EMarioForm : uint8
{
	Small		UMETA(DisplayName = "Small"),
	Super		UMETA(DisplayName = "Super"),
	Fire		UMETA(DisplayName = "Fire"),
	Gun			UMETA(DisplayName = "Gun"),
	Rocket		UMETA(DisplayName = "Rocket"),
	Raccoon		UMETA(DisplayName = "Raccoon")
};

UENUM(BlueprintType)
enum class EMarioDirection : uint8
{
	Right	UMETA(DisplayName = "Right"),
	Left	UMETA(DisplayName = "Left")
};

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API AMarioPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMarioPlayerState();

	float DirectionScalar();

	EMarioState State;
	EMarioForm Form;
	EMarioDirection Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario")
	int32 RunningMeter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario")
	int32 Coins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario")
	int32  Lives;

	float InvincibilityTimer;

	bool IsOnGround;
	bool IsRunning;
};

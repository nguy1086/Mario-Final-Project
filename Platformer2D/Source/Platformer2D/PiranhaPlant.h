// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "PiranhaPlant.generated.h"

UENUM(BlueprintType)
enum class EPiranhaPlantState : uint8
{
	Unknown			UMETA(DisplayName = "Unknown"),
	Hiding			UMETA(DisplayName = "Hiding"),
	AnimatingUp		UMETA(DisplayName = "AnimatingUp"),
	Active			UMETA(DisplayName = "Active"),
	AnimatingDown	UMETA(DisplayName = "AnimatingDown")
};

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API APiranhaPlant : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APiranhaPlant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SetState(EPiranhaPlantState state);

	EPiranhaPlantState State;
	FVector HidingLocation;
	FVector ActiveLocation;
	float HoldTimer;
	float AnimationTimer;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Coin		UMETA(DisplayName = "Coin"),
	Mushroom	UMETA(DisplayName = "Mushroom"),
	OneUp		UMETA(DisplayName = "OneUp"),
	Flower		UMETA(DisplayName = "Flower"),
	Gun			UMETA(DisplayName = "Gun"),
	Rocket		UMETA(DisplayName = "Rocket"),
	Leaf		UMETA(DisplayName = "Leaf")
};

UENUM(BlueprintType)
enum class EPickupState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Spawning	UMETA(DisplayName = "Spawning"),
	Active		UMETA(DisplayName = "Active")
};

UCLASS()
class PLATFORMER2D_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UPaperFlipbookComponent* FlipbookComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EPickupType GetPickupType();

	void Spawn();

protected:
	virtual void OnPickupActive();

	EPickupType Type;
	EPickupState State;
	FVector Start;
	FVector Target;
	float SpawnTime;
};

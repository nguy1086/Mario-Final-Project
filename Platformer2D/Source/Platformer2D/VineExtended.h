// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EVineExtendedState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Spawning	UMETA(DisplayName = "Spawning"),
	Active		UMETA(DisplayName = "Active")
};

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VineExtended.generated.h"

UCLASS()
class PLATFORMER2D_API AVineExtended : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVineExtended();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UPaperFlipbookComponent* FlipbookComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Spawn();

private:
	FVector Start;
	FVector Target;
	float SpawnTime;
	EVineExtendedState State;
};

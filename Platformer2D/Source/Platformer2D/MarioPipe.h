// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

enum class EWarpState : uint8
{
	Unknown		UMETA(DisplayName "Unknown"),
	Warping		UMETA(DisplayName "Warping")
};

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MarioPipe.generated.h"

UCLASS()
class PLATFORMER2D_API AMarioPipe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarioPipe();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Counterpart")
	class ADoor* Counterpart;

	void DoorAction();
public:	
	void BeginPlay();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector Start;
	FVector Target;
	float AnimationTimer;

	EWarpState State;
	class AMarioCharacter* Mario;
};

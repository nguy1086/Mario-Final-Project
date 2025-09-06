// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Unknown			UMETA(DisplayName = "Unknown"),
	PiranhaPlant	UMETA(DisplayName = "PiranhaPlant"),
	Goomba			UMETA(DisplayName = "Goomba"),
	Boo				UMETA(DisplayName = "Boo"),
	Thwomp			UMETA(DisplayName = "Thwomp"),
	Podoboo			UMETA(DisplayName = "Podoboo"),
	DryBones		UMETA(DisplayName = "DryBones"),
	BoomBoom		UMETA(DisplayName = "BoomBoom"),
	Bobomb			UMETA(DiplayName = "Bobomb"),
	Muncher			UMETA(DisplayName = "Muncher"),
	Nipper			UMETA(DisplayName = "Nipper"),
	Spike			UMETA(DisplayName = "Spike")
};

UCLASS()
class PLATFORMER2D_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPaperFlipbookComponent* FlipbookComponent;

	EEnemyType GetEnemyType();

protected:
	EEnemyType Type;
};

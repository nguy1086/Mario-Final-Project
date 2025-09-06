// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EVineState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Spawning	UMETA(DisplayName = "Spawning"),
	Active		UMETA(DisplayName = "Active")
};

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vine.generated.h"

UCLASS()
class PLATFORMER2D_API AVine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVine();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnyWhere, Category = "Vine templates")
	TSubclassOf<class AVineExtended> VineExtendedTemplate;

	void DoorAction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Spawn();

private:
	FVector Original;
	FVector Start;
	FVector Target;
	float SpawnTime;
	EVineState State;
	float ZIncrement;
};

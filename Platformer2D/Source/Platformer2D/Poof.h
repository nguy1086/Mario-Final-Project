// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Poof.generated.h"

UCLASS()
class PLATFORMER2D_API APoof : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APoof();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UPaperFlipbookComponent* FlipbookComponent;

	void Spawn();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float LifeTime;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	FVector Start;
};

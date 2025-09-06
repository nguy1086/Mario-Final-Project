// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Goomba.generated.h"

UENUM(BlueprintType)
enum class EGoombaState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Walking		UMETA(DisplayName = "Walking"),
	Squished	UMETA(DisplayName = "Squished"),
	Exploded	UMETA(DisplayName = "Exploded")
};

UENUM(BlueprintType)
enum class EGoombaInitialDirection : uint8
{
	Random		UMETA(DisplayName = "Random"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right")
};

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API AGoomba : public AEnemy
{
	GENERATED_BODY()

public:
	AGoomba();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	EGoombaInitialDirection InitialDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* FrontOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BackOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goomba flipbooks")
	class UPaperFlipbook* WalkingFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goomba flipbooks")
	class UPaperFlipbook* SquishedFlipbook;

	UPROPERTY(EditAnyWhere, Category = "Gib template")
	TSubclassOf<class AGib> GibTemplate;

	UPROPERTY(EditAnyWhere, Category = "Blood template")
	TSubclassOf<class ABlood> BloodTemplate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetState(EGoombaState state);

protected:
	UFUNCTION()
	void OnForwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBackwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	void ApplyInitialVelocity();
	void UpdateFlipbook();

	FVector Velocity;
	float SquishedDuration;
	EGoombaState State;
};

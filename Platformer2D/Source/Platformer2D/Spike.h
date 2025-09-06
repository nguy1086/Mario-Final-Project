// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Spike.generated.h"

UENUM(BlueprintType)
enum class ESpikeState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Walking		UMETA(DisplayName = "Walking"),
	Reloading	UMETA(DisplayName = "Reloading"),
	Aimming		UMETA(DisplayName = "Aimming"),
	Squished	UMETA(DisplayName = "Squished"),
	Exploded	UMETA(DisplayName = "Exploded")
};

UENUM(BlueprintType)
enum class ESpikeInitialDirection : uint8
{
	Random		UMETA(DisplayName = "Random"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right")
};
/**
 * 
 */
UCLASS()
class PLATFORMER2D_API ASpike : public AEnemy
{
	GENERATED_BODY()
	
public:
	ASpike();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	ESpikeInitialDirection InitialDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* FrontOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BackOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike flipbooks")
	class UPaperFlipbook* WalkingFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike flipbooks")
	class UPaperFlipbook* ReloadingFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike flipbooks")
	class UPaperFlipbook* AimmingFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike flipbooks")
	class UPaperFlipbook* SquishedFlipbook;

	UPROPERTY(EditAnyWhere, Category = "BulletBill templates")
	TSubclassOf<class ASpikeBall> SpikeBallTemplate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetState(ESpikeState state);

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
	float WalkingDuration;
	float ReloadingDuration;
	float AimmingDuration;
	ESpikeState State;
};

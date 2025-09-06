// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Bobomb.generated.h"
UENUM(BlueprintType)
enum class EBobombState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Walking		UMETA(DisplayName = "Walking"),
	Stunned		UMETA(DisplayName = "Stunned"),
	Exploded	UMETA(DisplayName = "Exploded")
};

UENUM(BlueprintType)
enum class EBobombInitialDirection : uint8
{
	Random		UMETA(DisplayName = "Random"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right")
};
/**
 * 
 */
UCLASS()
class PLATFORMER2D_API ABobomb : public AEnemy
{
	GENERATED_BODY()
public:
	ABobomb();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	EBobombInitialDirection InitialDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* FrontOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BackOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goomba flipbooks")
	class UPaperFlipbook* WalkingFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goomba flipbooks")
	class UPaperFlipbook* StunnedFlipbook;

	UPROPERTY(EditAnyWhere, Category = "Explosion template")
	TSubclassOf<class AExplosion> ExplosionTemplate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetState(EBobombState state);
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
	float StunnedDuration;
	EBobombState State;
};

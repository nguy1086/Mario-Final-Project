// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "SpikeBall.generated.h"

UENUM(BlueprintType)
enum class ESpikeBallState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Spawning	UMETA(DisplayName = "Spawning"),
	Aimming		UMETA(DisplayName = "Aimming"),
	Active		UMETA(DisplayName = "Active")
};

UCLASS()
class PLATFORMER2D_API ASpikeBall : public AEnemy
{
	GENERATED_BODY()
	
public:	
	ASpikeBall();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* CollisionCheck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpikeBall flipbooks")
	class UPaperFlipbook* IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpikeBall flipbooks")
	class UPaperFlipbook* ActiveFlipbook;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnCheckOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:	
	virtual void Tick(float DeltaTime) override;

	void Spawn(float direction);

private:
	void SetState(ESpikeBallState state);
	void UpdateFlipbook();

	FVector Velocity;
	ESpikeBallState State;
	FVector Start;
	FVector Target;

	float SpawningDuration;
	float AimmingDuration;
	float LifeTime;
	float Turn;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "BoomBoom.generated.h"

UENUM(BlueprintType)
enum class EBoomBoomState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Dormant		UMETA(DisplayName = "Dormant"),
	Walking		UMETA(DisplayName = "Walking"),
	Idle		UMETA(DisplayName = "Idle"),
	Hurt		UMETA(DisplayName = "Hurt"),
	Dead		UMETA(DisplayName = "Dead"),
	Jump		UMETA(DisplayName = "Jump"),
	Falling		UMETA(DisplayName = "Falling"),
	Exploded	UMETA(DisplayName = "Exploded")
};

UENUM(BlueprintType)
enum class EBoomBoomDirection : uint8
{
	Random		UMETA(DisplayName = "Random"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right")
};

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API ABoomBoom : public AEnemy
{
	GENERATED_BODY()
public:
	ABoomBoom();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	EBoomBoomDirection InitialDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* FrontOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BackOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BottomOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoomBoom flipbooks")
	class UPaperFlipbook* WalkingFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoomBoom flipbooks")
	class UPaperFlipbook* IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoomBoom flipbooks")
	class UPaperFlipbook* HurtFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoomBoom flipbooks")
	class UPaperFlipbook* DeadFlipbook;

	UPROPERTY(EditAnyWhere, Category = "Question template")
	TSubclassOf<class AQuestionCircle> QuestionTemplate;

	UPROPERTY(EditAnyWhere, Category = "Star template")
	TSubclassOf<class AStar> StarTemplate;

	UPROPERTY(EditAnyWhere, Category = "Gib template")
	TSubclassOf<class AGib> GibTemplate;

	UPROPERTY(EditAnyWhere, Category = "Blood template")
	TSubclassOf<class ABlood> BloodTemplate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnForwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBackwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBottomOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	virtual void Tick(float DeltaTime) override;

	void SetState(EBoomBoomState state);

private:
	void ApplyInitialVelocity();
	void UpdateFlipbook();
	void SpawnQuestionMark();

	FVector Velocity;

	float IdleDuration;
	float WalkDuration;
	float HurtDuration;
	float DeadDuration;

	int Lives;
	int FireballHits;
	float SpeedIncrement;

	float WalkSwapDuration;
	float JumpDelay;

	EBoomBoomState State;
	FVector OriginalPosition;
};

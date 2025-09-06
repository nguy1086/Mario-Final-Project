// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "DryBones.generated.h"


UENUM(BlueprintType)
enum class EDryBonesState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Walking		UMETA(DisplayName = "Walking"),
	Fallen		UMETA(DisplayName = "Fallen"),
	Stunned		UMETA(DisplayName = "Stunned"),
	Stirring	UMETA(DisplayName = "Stirring"),
	Shotted		UMETA(DisplayName = "Shotted"),
	Exploded	UMETA(DisplayName = "Exploded")
};

UENUM(BlueprintType)
enum class EDryBonesInitialDirection : uint8
{
	Random		UMETA(DisplayName = "Random"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right")
};
/**
 * 
 */
UCLASS()
class PLATFORMER2D_API ADryBones : public AEnemy
{
	GENERATED_BODY()
	
public:
	ADryBones();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	EDryBonesInitialDirection InitialDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* FrontOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BackOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DryBones flipbooks")
	class UPaperFlipbook* WalkingFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DryBones flipbooks")
	class UPaperFlipbook* FallenFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DryBones flipbooks")
	class UPaperFlipbook* StunnedFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DryBones flipbooks")
	class UPaperFlipbook* StirringFlipbook;

	UPROPERTY(EditAnyWhere, Category = "Gib template")
	TSubclassOf<class AGib> GibTemplate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetState(EDryBonesState state);
	void AddFlipBookOffset();
	void SubtractFlipbookOffset();

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
	float FallenDuration;
	float StunnedDuration;
	float StirringDuration;
	EDryBonesState State;
};

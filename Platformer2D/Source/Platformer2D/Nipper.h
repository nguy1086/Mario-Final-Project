// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Nipper.generated.h"

UENUM(BlueprintType)
enum class ENipperState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Active		UMETA(DisplayName = "Active =="),
	Squished	UMETA(DisplayName = "Squished")
};

UENUM(BlueprintType)
enum class ENipperDirection : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right"),
	Up			UMETA(DisplayName = "Up")
};

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API ANipper : public AEnemy
{
	GENERATED_BODY()
	
public:
	ANipper();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goomba flipbooks")
	class UPaperFlipbook* StraightFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goomba flipbooks")
	class UPaperFlipbook* UpFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* Check;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CheckHit(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Tick(float DeltaTime) override;

	void SetState(ENipperState state);
	void SetDirection(ENipperDirection direction);

private:
	void UpdateFlipbook();

	ENipperState State;
	FVector Velocity;
	FVector OriginalLocation;
	ENipperDirection Direction;
};

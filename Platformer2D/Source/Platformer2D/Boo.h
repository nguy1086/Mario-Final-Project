// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Boo.generated.h"

UENUM(BlueprintType)
enum class EBooState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Active		UMETA(DisplayName = "Active"),
	Inactive	UMETA(DisplayName = "Inactive"),
	Shotted		UMETA(DisplayName = "Shotted")
};

UENUM(BlueprintType)
enum class EBooDirection : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right")
};

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API ABoo : public AEnemy
{
	GENERATED_BODY()

public:
	ABoo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boo flipbooks")
	class UPaperFlipbook* ActiveFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boo flipbooks")
	class UPaperFlipbook* InactiveFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* EnemyCollisionCheck;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnEnemyCollision(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	virtual void Tick(float DeltaTime) override;

private:
	void SetState(EBooState state);
	void SetDirection(EBooDirection dir);
	class AMarioPlayerState* PlayerState;
	void UpdateFlipbook();
	bool Shotted;

	FVector MarioLocation;
	FVector Velocity;
	EBooState State;
	EBooDirection Direction;
};

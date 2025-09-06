// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Podoboo.generated.h"

enum class EPodobooState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Waiting		UMETA(DisplayName = "Waiting"),
	Jumping		UMETA(DisplayName = "Jumping"),
	Dropping	UMETA(DisplayName = "Dropping"),
	Shotted		UMETA(DisplayName = "Shotted")
};

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API APodoboo : public AEnemy
{
	GENERATED_BODY()

public:
	APodoboo();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* CheckOverlap;

	UPROPERTY(EditAnyWhere, Category = "Splash template")
	TSubclassOf<class ASplash> SplashTemplate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void Overlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Tick(float DeltaTime) override;

	void SetState(EPodobooState state);

private:
	FVector Velocity;
	EPodobooState State;
	FVector OriginalLocation;
	float Apex;

	float WaitTimer;
	float SplashDelay;
};

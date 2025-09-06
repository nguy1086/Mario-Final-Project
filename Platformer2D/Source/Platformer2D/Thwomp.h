// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Thwomp.generated.h"

enum class EThwompState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Active		UMETA(DisplayName = "Active"),
	Dropping	UMETA(DisplayName = "Dropping"),
	Delay		UMETA(DisplayName = "Delay"),
	Raise		UMETA(DisplayName = "Raise"),
	Shotted		UMETA(DisplayName = "Shotted"),
	Exploded	UMETA(DisplayName = "Exploded")
};
/**
 * 
 */
UCLASS()
class PLATFORMER2D_API AThwomp : public AEnemy
{
	GENERATED_BODY()
	
public:
	AThwomp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* WorldCollisionCheck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thwomp audio")
	class USoundBase* ThwompSound;

	UPROPERTY(EditAnyWhere, Category = "Gib template")
	TSubclassOf<class AGib> GibTemplate;

	UPROPERTY(EditAnyWhere, Category = "Blood template")
	TSubclassOf<class ABlood> BloodTemplate;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnWorldCollision(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Tick(float DeltaTime) override;

private:
	void SetState(EThwompState state);

	FVector Velocity;
	FVector OriginalLocation;
	FVector LandedLocation;

	float Delay;
	EThwompState State;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "BulletBill.generated.h"
UENUM(BlueprintType)
enum class EBulletBillState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Peeking		UMETA(DisplayName = "Peeking"),
	Aimming		UMETA(DisplayName = "Aimming"),
	Active		UMETA(DisplayName = "Active"),
	Squished	UMETA(DisplayName = "Squished")
};

UENUM(BlueprintType)
enum class EBulletBillDirection : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right")
};
/**
 * 
 */
UCLASS()
class PLATFORMER2D_API ABulletBill : public AEnemy
{
	GENERATED_BODY()
	
public:
	ABulletBill();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnCheckOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTopOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* CollisionCheck;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* TopCheck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletBill audio")
	class USoundBase* ShootSound;

	UPROPERTY(EditAnyWhere, Category = "BulletBill templates")
	TSubclassOf<class APoof> PoofTemplate;

public:
	virtual void Tick(float DeltaTime) override;

	void Spawn(float direction);

private:
	void SetState(EBulletBillState state);

	FVector Velocity;
	EBulletBillState State;
	EBulletBillDirection Direction;

	float Turn;
	float PeekingTimer;
	float AimmingTimer;
	float LifeTime;
};

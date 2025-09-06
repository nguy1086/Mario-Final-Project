// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

enum class EDiscState :uint8
{
	Unknown		UMETA(DisplayName "Unknown"),
	Active		UMETA(DisplayName "Active"),
	Shotted		UMETA(DisplayName "Shotted")
};

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Disc.generated.h"


UCLASS()
class PLATFORMER2D_API ADisc : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADisc();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* CheckOverlap;

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void Overlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Spawn(float radius, FVector center);

private:
	FVector Velocity;
	float Radius;
	float Angle;
	FVector Center;
	EDiscState State;
};

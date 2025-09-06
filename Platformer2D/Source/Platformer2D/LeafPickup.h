// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "LeafPickup.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API ALeafPickup : public APickup
{
	GENERATED_BODY()

public:
	ALeafPickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* Overlap;

	virtual void Tick(float DeltaTime) override;

	void Spawn();

protected:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnPickupActive() override;

private:
	FVector Velocity;
	float Angle;
};

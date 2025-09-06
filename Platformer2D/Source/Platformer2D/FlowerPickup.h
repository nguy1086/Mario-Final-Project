// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "FlowerPickup.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API AFlowerPickup : public APickup
{
	GENERATED_BODY()
public:
	AFlowerPickup();
	
protected:
	void OnPickupActive() override;
};

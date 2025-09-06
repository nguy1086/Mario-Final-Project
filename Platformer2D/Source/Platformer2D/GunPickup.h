// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "GunPickup.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API AGunPickup : public APickup
{
	GENERATED_BODY()
public:
	AGunPickup();

protected:
	void OnPickupActive() override;
};

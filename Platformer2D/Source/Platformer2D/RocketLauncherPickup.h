// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "RocketLauncherPickup.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API ARocketLauncherPickup : public APickup
{
	GENERATED_BODY()
public:
	ARocketLauncherPickup();
protected:
	void OnPickupActive() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "CoinPickup.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API ACoinPickup : public APickup
{
	GENERATED_BODY()
	
public:
	ACoinPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoinPickup flipbooks")
	class UPaperFlipbook* StaticCoinFlipbook;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void UseStaticCoin();
	void UseMuncherCoin();
};

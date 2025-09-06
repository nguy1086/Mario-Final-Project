// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PlatformerGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API APlatformerGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	APlatformerGameStateBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float TimeRemaining;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float BlackOverlayAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool IsGameOver;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool IsWon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool IsPaused;
};

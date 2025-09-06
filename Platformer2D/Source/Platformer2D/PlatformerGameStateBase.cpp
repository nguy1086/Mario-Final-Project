// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerGameStateBase.h"
#include "GameConstants.h"


APlatformerGameStateBase::APlatformerGameStateBase() :
	TimeRemaining(GameConstants::DefaultGameDuration),
	BlackOverlayAlpha(0.0f),
	IsGameOver(false),
	IsWon(false),
	IsPaused(false)
{}

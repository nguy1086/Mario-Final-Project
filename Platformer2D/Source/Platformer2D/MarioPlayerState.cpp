// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioPlayerState.h"
#include "MarioConstants.h"


AMarioPlayerState::AMarioPlayerState()
{
	State = EMarioState::Idle;
	Form = EMarioForm::Small;
	Direction = EMarioDirection::Right;

	RunningMeter = 0;
	Coins = 0;
	Lives = MarioConstants::DefaultStartingLives;

	InvincibilityTimer = 0.0f;

	IsOnGround = true;
	IsRunning = false;
}

float AMarioPlayerState::DirectionScalar()
{
	return Direction == EMarioDirection::Left ? -1.0f : 1.0f;
}


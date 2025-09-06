// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

namespace MarioConstants
{
	// General constants
	constexpr int DefaultStartingLives = 4;
	constexpr int MaxRunningMeter = 7;
	constexpr float InvincibleTime = 1.5f;
	constexpr float InvincibleVisibilityDuration = 0.05f;


	// Jump constants
	constexpr float GravityScale = 3.75f;
	constexpr float AirControl = 1.0f;
	constexpr float JumpZVelocity = 750.0f;
	constexpr float JumpMaxHoldTimeWalking = 0.15f;
	constexpr float JumpMaxHoldTimeRunning = 0.19f;
	constexpr float JumpIncreasePerSegment = 0.0025f;


	// Walking / Running constants
	constexpr float GroundFriction = 3.5f;
	constexpr float MinWalkSpeed = 300.0f;
	constexpr float MaxWalkSpeed = 400.0f;
	constexpr float MaxAcceleration = 900.0f;
	constexpr float RunSpeedPerSegment = 35.0f;
	constexpr float RunSegmentIncrementDuration = 0.2f;
	constexpr float RunSegmentDecrementDuration = 0.05f;
	constexpr float MaxStepHeight = 10.0f;


	// Collision
	constexpr float CapsuleRadius = 14.0f;
	constexpr float CapsuleHalfHeightSmall = 14.0f;
	constexpr float CapsuleHalfHeightSuper = 25.0f;


	// Dead
	constexpr float DeadHoldTime = 1.5f;
	constexpr float DeadImpulseZ = 600.0f;
}

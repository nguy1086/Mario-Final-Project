// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

namespace GameConstants
{
	// Gameplay constants
	constexpr float DestroyActorAtZ = -250.0f;
	constexpr float DefaultGameDuration = 300.0f;
	constexpr float BlackOverlayFadeInOutDuration = 0.7f;
	constexpr float BlackOverlayFadeHoldDuration = 0.1f;
	constexpr float BreakableBlockCoinSwitchDuration = 10.0f;

	constexpr float WonFadeDuration = 0.7f;

	// Tile constants
	constexpr float TileSize = 32.0f;
	constexpr float TileHalfSize = TileSize * 0.5f;


	// Resolution and Camera constants
	constexpr float ResolutionWidth = 640.0f;
	constexpr float ResolutionHeight = 480.0f;
	constexpr float ResolutionAspectRatio = ResolutionWidth / ResolutionHeight;
	constexpr float CameraDepthY = 200.0f;
	constexpr float CameraRotationYaw = 270.0f;
	constexpr int32 MaxCameraShakes = 9;

	// Donut constants
	constexpr float DonutTimer = 2.0f;
	constexpr float DonutDropSpeed = 300.0f;
	constexpr float DonutShakeXMin = -3.0f;
	constexpr float DonutShakeXMax = 3.0f;

	//Trap wall constants
	constexpr float WallSpeed = 600.0f;

	//Star constants
	constexpr float StarSpeed = 50.0f;
	constexpr float StarTimer = 1.5f;

	//Fireball constants
	constexpr float FireballSpeed = 300.0f;
	constexpr float FireballCooldown = 0.2f;
	constexpr float FireballLifeTime = 2.0f;
	
	//Bullet constants
	constexpr float BulletSpeed = 1100.0f;
	constexpr float RocketSpeed = 500.0f;
	constexpr float RocketIncrementSpeed = 25.0f;
}

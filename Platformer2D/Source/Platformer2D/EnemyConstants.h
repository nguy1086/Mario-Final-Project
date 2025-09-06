// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

namespace EnemyConstants
{
	// Piranha Plant constants
	constexpr float PiranhaPlantOffsetZ = 48.0f;
	constexpr float PiranhaPlantAnimationDuration = 0.75f;
	constexpr float PiranhaPlantActiveDuration = 2.5f;
	constexpr float PiranhaPlantHiddenDurationMin = 2.0f;
	constexpr float PiranhaPlantHiddenDurationMax = 4.0f;

	// Goomba constants
	constexpr float GoombaSpeed = 100.0f;
	constexpr float GoombaSquishedDuration = 0.6f;

	// Boo constants
	constexpr float BooSpeed = 80.0f;

	//Thwomp constants
	constexpr float ThwompDropSpeed = -600.0f;
	constexpr float ThwompRaiseSpeed = 30.0f;
	constexpr float ThwompDelayTimer = 1.25f;
	constexpr float ThwompRange = 75.0f;
	constexpr float ThwompDropCheck = 2.0f;

	//Podoboo constants
	constexpr float PodobooJumpMin = 3.0f;
	constexpr float PodobooJumpMax = 4.5f;
	constexpr float PodobooSpeed = 350.0f;
	constexpr float PodobooApexMin = 256.0f;
	constexpr float PodobooApexMax = 288.0f;
	constexpr float PodobooMinSpeed = 50.0f;

	//Disc constants
	constexpr float DiscSpeed = 150.0f;

	//Dry Bones constants
	constexpr float DryBonesFallenDuration = 1.25f;
	constexpr float DryBonesStunnedDuration = 8.0f;
	constexpr float DryBonesStirringDuration = 1.75f;
	constexpr float DryBonesSpeed = 90.0f;

	//BoomBoom constants
	constexpr int BoomBoomLives = 3;
	constexpr int BoomBoomFireballHits = 5;
	constexpr float BoomBoomSpeed = 100.0f;

	constexpr float BoomBoomWalkingTimer = 8.0f;
	constexpr float BoomBoomIdleTimer = 1.5f;
	constexpr float BoomBoomHurtTimer = 2.0f;
	constexpr float BoomBoomDeadTimer = 5.0f;

	constexpr float BoomBoomSwapWalkMin = 1.0f;
	constexpr float BoomBoomSwapWalkMax = 3.0f;

	constexpr float BoomBoomJumpDelay = 1.0f;
	constexpr float BoomBoomJumpSpeed = 800.0f;
	//constexpr float BoomBoomJumpApex = 400.0f;

	//Bullet bill constants
	constexpr float BulletBillSpeed = 150.0f;
	constexpr float BulletBillPeekingTimer = 0.1f;
	constexpr float BulletBillAimmingTimer = 1.5f;
	constexpr float BulletBillCooldown = 3.5f;
	constexpr float BulletBillLifeTime = 6.0f;

	//Bobomb constants
	constexpr float BobombStunnedDuration = 3.0f;
	constexpr float BobombSpeed = 95.0f;

	//Nipper constants
	constexpr float NipperApex = 300.0f;
	constexpr float NipperSpeed = 300.0f;

	//Spike constants
	constexpr float SpikeToReloadMin = 4.0f;
	constexpr float SpikeToReloadMax = 5.0f;
	constexpr float SpikeReloading = 0.5f;
	constexpr float SpikeAimming = 0.25f;

	constexpr float SpikeBallSpeed = 250.0f;
	constexpr float SpikeBallLifeTime = 10.0f;
}

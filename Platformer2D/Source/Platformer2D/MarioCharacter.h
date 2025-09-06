// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "MarioCharacter.generated.h"

// Forward declaration
enum class EMarioState : uint8;
enum class EMarioForm : uint8;

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API AMarioCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	AMarioCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SmallIdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SmallWalkFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SmallRunFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SmallJumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SmallRunJumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SmallTurnFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SuperIdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SuperWalkFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SuperRunFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SuperJumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SuperFallingFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SuperRunJumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SuperTurnFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* SuperDuckFlipbook;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* GunIdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* GunWalkFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* GunRunFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* GunJumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* GunFallingFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* GunRunJumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* GunTurnFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* GunDuckFlipbook;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RocketIdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RocketWalkFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RocketRunFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RocketJumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RocketFallingFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RocketRunJumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RocketTurnFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RocketDuckFlipbook;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* FireIdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* FireWalkFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* FireRunFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* FireJumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* FireFallingFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* FireRunJumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* FireTurnFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* FireDuckFlipbook;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RaccoonIdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RaccoonWalkFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RaccoonRunFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RaccoonJumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RaccoonFallingFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RaccoonRunJumpFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RaccoonTurnFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RaccoonDuckFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RaccoonGlideFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RaccoonFlyFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* RaccoonSpinFlipbook;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario audio")
	class USoundBase* JumpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario audio")
	class USoundBase* DiedSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario audio")
	class USoundBase* CoinSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario audio")
	class USoundBase* OneUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario audio")
	class USoundBase* PowerUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario audio")
	class USoundBase* PowerDownSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario audio")
	class USoundBase* StompSound;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Respawn(FVector location);

	void Run();
	void StopRunning();

	void Duck();
	void StopDucking();

	void TryDoor();

	void SetJumpHoldTime();

	void ApplyStateChange(EMarioState newState);
	void ApplyTransformChange(EMarioForm newForm, bool noAnimation = false);
	void ApplyBounce();

	void HandleDamage(bool forceDead = false);
	bool IsInvincible();

	void SpawnFireball();

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegan(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnded(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnJumped_Implementation() override;
	void NotifyJumpApex() override;
	void Landed(const FHitResult& Hit) override;

private:
	void MarioHasDied(bool spawnDeadMario);

	void UpdateFlipbook();

	UPaperFlipbook* GetSmallMarioFlipbook();
	UPaperFlipbook* GetSuperMarioFlipbook();
	UPaperFlipbook* GetFireMarioFlipbook();
	UPaperFlipbook* GetGunMarioFlipbook();
	UPaperFlipbook* GetRocketMarioFlipbook();
	UPaperFlipbook* GetRaccoonMarioFlipbook();

	class AMarioPlayerState* PlayerState;
	class AMarioCamera* CurrentCamera;
	class ADoor* CurrentDoor;
	class AVine* CurrentVine;
	class AMarioPipe* CurrentPipe;
	class AFireball* PlayerFireball;
	float RunningSegmentTimer;
	float DamagedTimer;
	bool IsRunningBlocked;	
};

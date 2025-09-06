// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MarioCamera.generated.h"

UCLASS()
class PLATFORMER2D_API AMarioCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarioCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* TileMapConfinesBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyCameraShake();

	void SetCameraLocation(FVector2D location);
	void SetTileMapSize(int32 columns, int32 rows);

private:
	void OnShake();

	class AMarioCharacter* MarioCharacter;
	FVector InitialLocation;
	FVector ShakeOffset;
	FTimerHandle ShakeTimer;
	int32 ShakeCount;

	float OriginalZ;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gib.generated.h"

UENUM(BlueprintType)
enum class EGibType : uint8
{
	Unknown			UMETA(DisplayName = "Unknown"),
	BoomBoomHead	UMETA(DisplayName = "BoomBoomHead"),
	BoomBoomArm		UMETA(DisplayName = "BoomBoomArm"),
	ThwompEnd		UMETA(DisplayName = "ThwompEnd"),
	ThwompMiddle	UMETA(DisplayName = "ThwompMiddle"),
	GoombaHead		UMETA(DisplayName = "GoombaHead"),
	GoombaFoot		UMETA(DisplayName = "GoombaFoot"),
	DryBonesHead	UMETA(DisplayName = "DryBonesHead"),
	Bone			UMETA(DisplayName = "Bone"),
	ToadHead		UMETA(DisplayName = "ToadHead"),
	ToadArm			UMETA(DisplayName = "ToadArm")
};

UCLASS()
class PLATFORMER2D_API AGib : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGib();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class USphereComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gib flipbooks")
	class UPaperSprite* BoomBoomHeadSprite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gib flipbooks")
	class UPaperSprite* BoomBoomArmSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gib flipbooks")
	class UPaperSprite* ThwompEndSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gib flipbooks")
	class UPaperSprite* ThwompMiddleSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gib flipbooks")
	class UPaperSprite* GoombaHeadSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gib flipbooks")
	class UPaperSprite* GoombaFootSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gib flipbooks")
	class UPaperSprite* DryBonesHeadSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gib flipbooks")
	class UPaperSprite* BoneSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gib flipbooks")
	class UPaperSprite* ToadHeadSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gib flipbooks")
	class UPaperSprite* ToadArmSprite;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Spawn(EGibType type, FVector impulse);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableBlockBit.generated.h"


UENUM(BlueprintType)
enum class EBreakableBlockBitType : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right")
};

UCLASS()
class PLATFORMER2D_API ABreakableBlockBit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableBlockBit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class USphereComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprites")
	class UPaperSprite* LeftSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprites")
	class UPaperSprite* RightSprite;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Spawn(EBreakableBlockBitType type, FVector impulse);
};

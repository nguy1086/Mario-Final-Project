// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MarioTransform.generated.h"

enum class EMarioForm : uint8;
enum class EMarioDirection : uint8;

UCLASS()
class PLATFORMER2D_API AMarioTransform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarioTransform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Flipbook")
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* TransformFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* FireTransformFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mario flipbooks")
	class UPaperFlipbook* DamageFlipbook;

	void SetFlipbook(EMarioForm oldForm, EMarioForm newForm, EMarioDirection direction);

protected:
	UFUNCTION()
	void OnFinishedPlaying();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableBlock.generated.h"


UENUM(BlueprintType)
enum class EBreakableBlockState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Active		UMETA(DisplayName = "Active"),
	AnimUp		UMETA(DisplayName = "AnimUp"),
	AnimDown	UMETA(DisplayName = "AnimDown"),
	Breaking	UMETA(DisplayName = "Breaking")
};

UCLASS()
class PLATFORMER2D_API ABreakableBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableBlock();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnyWhere, Category = "BreakableBlock Templates")
	TSubclassOf<class ABreakableBlockBit> BreakableBlockBitTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breakable Block audio")
	class USoundBase* BlockSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool CanTransformToCoin();

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	void SetState(EBreakableBlockState state);

	FVector Start;
	FVector Target;
	FVector Original;
	float AnimationTimer;
	EBreakableBlockState State;
};

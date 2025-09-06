// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBox.generated.h"


UENUM(BlueprintType)
enum class EItemBoxState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Active		UMETA(DisplayName = "Active"),
	AnimUp		UMETA(DisplayName = "AnimUp"),
	AnimDown	UMETA(DisplayName = "AnimDown"),
	Spawning	UMETA(DisplayName = "Spawning"),
	Inactive	UMETA(DisplayName = "Inactive")
};

UENUM(BlueprintType)
enum class EItemBoxContents : uint8
{
	Coin1		UMETA(DisplayName = "Coin1"),
	Coin5		UMETA(DisplayName = "Coin5"),
	Coin10		UMETA(DisplayName = "Coin10"),
	Mushroom	UMETA(DisplayName = "Mushroom"),
	CoinSwitch	UMETA(DisplayName = "CoinSwitch"),
	OneUp		UMETA(DisplayName = "OneUp"),
	Flower		UMETA(DisplayName = "Flower"),
	Vine		UMETA(DisplayName = "Vine"),
	Gun			UMETA(DisplayName = "Gun"),
	Rocket		UMETA(DisplayName = "Rocket"),
	Leaf		UMETA(DisplayName = "Leaf")
};


UCLASS()
class PLATFORMER2D_API AItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBox();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Components")
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemBox flipbooks")
	class UPaperFlipbook* ActiveFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemBox flipbooks")
	class UPaperFlipbook* InactiveFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemBox state")
	EItemBoxContents Contents;

	UPROPERTY(EditAnyWhere, Category = "ItemBox templates")
	TSubclassOf<class AItemBoxCoin> ItemBoxCoinTemplate;

	UPROPERTY(EditAnyWhere, Category = "ItemBox templates")
	TSubclassOf<class AMushroomPickup> MushroomPickupTemplate;

	UPROPERTY(EditAnyWhere, Category = "ItemBox templates")
	TSubclassOf<class ACoinSwitch> CoinSwitchTemplate;

	UPROPERTY(EditAnyWhere, Category = "ItemBox templates")
	TSubclassOf<class AOneUpPickup> OneUpTemplate;

	UPROPERTY(EditAnyWhere, Category = "ItemBox templates")
	TSubclassOf<class AFlowerPickup> FlowerTemplate;

	UPROPERTY(EditAnyWhere, Category = "ItemBox templates")
	TSubclassOf<class AVine> VineTemplate;

	UPROPERTY(EditAnyWhere, Category = "ItemBox templates")
	TSubclassOf<class AGunPickup> GunTemplate;

	UPROPERTY(EditAnyWhere, Category = "ItemBox templates")
	TSubclassOf<class ARocketLauncherPickup> RocketLauncherTemplate;

	UPROPERTY(EditAnyWhere, Category = "ItemBox templates")
	TSubclassOf<class ALeafPickup> LeafTemplate;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemBox audio")
	class USoundBase* ItemSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemBox audio")
	class USoundBase* CoinSound;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsEmpty();

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool IsCoinItemBox();
	int16 CoinsLeft();

private:
	void SpawnPickup();
	void SetState(EItemBoxState state);

	FVector Start;
	FVector Target;
	FVector Original;
	float AnimationTimer;
	int16 CoinCount;
	EItemBoxState State;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Toad.generated.h"

UENUM(BlueprintType)
enum class EToadState : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Idle		UMETA(DisplayName = "Idle"),
	Jumping		UMETA(DisplayName = "Jumping"),
	Shotted		UMETA(DisplayName = "Shotted"),
	Exploded	UMETA(DisplayName = "Exploded")
};


UCLASS()
class PLATFORMER2D_API AToad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToad();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* CheckOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoomBoom flipbooks")
	class UPaperFlipbook* IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoomBoom flipbooks")
	class UPaperFlipbook* JumpingFlipbook;

	UPROPERTY(EditAnyWhere, Category = "Gib template")
	TSubclassOf<class AGib> GibTemplate;

	UPROPERTY(EditAnyWhere, Category = "Blood template")
	TSubclassOf<class ABlood> BloodTemplate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnCheckOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetState(EToadState state);

private:
	void UpdateFlipbook();
	EToadState State;
	FVector Velocity;
};

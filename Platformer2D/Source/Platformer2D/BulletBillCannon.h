// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletBillCannon.generated.h"

UCLASS()
class PLATFORMER2D_API ABulletBillCannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletBillCannon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnyWhere, Category = "BulletBill templates")
	TSubclassOf<class ABulletBill> BulletBillTemplate;

	UPROPERTY(EditAnyWhere, Category = "BulletBillTop templates")
	TSubclassOf<class ABulletBillTop> BulletBillTopTemplate;

	UPROPERTY(EditAnyWhere, Category = "BulletBillBottom templates")
	TSubclassOf<class ABulletBillBottom> BulletBillBottomTemplate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float Spawn;
	float Direction;
	FVector CannonPosition;
};

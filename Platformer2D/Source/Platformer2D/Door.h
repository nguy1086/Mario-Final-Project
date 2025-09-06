// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"


UENUM(BlueprintType)
enum class EDoorType : uint8
{
	Unknown		UMETA(DisplayName = "Unknown"),
	Exit		UMETA(DisplayName = "Exit"),
	Entrance	UMETA(DisplayName = "Entrance")
};

UCLASS()
class PLATFORMER2D_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Counterpart")
	ADoor* Counterpart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Type")
	EDoorType Type;

	void DoorAction();

};

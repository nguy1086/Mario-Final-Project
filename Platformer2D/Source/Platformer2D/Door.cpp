// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "PlatformerGameModeBase.h"
#include "Components/BoxComponent.h"


// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the BoxComponent and set it as the Root component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("DoorBoxComponent");
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionProfileName("OverlapAll");
	BoxComponent->SetGenerateOverlapEvents(true);
	RootComponent = BoxComponent;

	Tags.Add("Door");
}

void ADoor::DoorAction()
{
	if (Type == EDoorType::Exit)
	{
		APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
		if (gameMode != nullptr)
		{
			gameMode->NextRoom(Counterpart);
		}
	}
}


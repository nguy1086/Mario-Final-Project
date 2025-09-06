// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBoxCoin.h"
#include "PaperFlipbookComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AItemBoxCoin::AItemBoxCoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USphereComponent>("Root");
	Root->SetVisibility(false);
	Root->SetCollisionProfileName("OverlapAll");
	Root->SetSimulatePhysics(true);
	RootComponent = Root;

	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>("PickupFlipbook");
	Flipbook->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AItemBoxCoin::BeginPlay()
{
	Super::BeginPlay();

	Start = GetActorLocation();
	Flipbook->PlayFromStart();
	Root->AddImpulse(FVector(0.0f, 0.0f, 350.0f));
}

// Called every frame
void AItemBoxCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().Z < Start.Z)
	{
		Destroy();
	}
}


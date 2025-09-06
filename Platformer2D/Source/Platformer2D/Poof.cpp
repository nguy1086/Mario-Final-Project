// Fill out your copyright notice in the Description page of Project Settings.


#include "Poof.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "GameConstants.h"

// Sets default values
APoof::APoof() :
	LifeTime(1.0f),
	Start(0.0f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("PickupFlipbookComponent");
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	Tags.Add("Poof");
}

// Called when the game starts or when spawned
void APoof::BeginPlay()
{
	Super::BeginPlay();

	FlipbookComponent->PlayFromStart();
}

// Called every frame
void APoof::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime -= DeltaTime;

	if (LifeTime < 0.0f)
	{
		Destroy();
	}
}

void APoof::Spawn()
{
	Start = GetActorLocation();
	Start.Y += 1.5f;
	SetActorLocation(Start);
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "Splash.h"
#include "PaperFlipbookComponent.h"
#include "GameConstants.h"
// Sets default values
ASplash::ASplash() :
	LifeTime(1.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("SplashFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASplash::BeginPlay()
{
	Super::BeginPlay();

	FlipbookComponent->PlayFromStart();
}

// Called every frame
void ASplash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime -= DeltaTime;
	if (LifeTime < 0.0f)
	{
		Destroy();
	}
}

void ASplash::SpawnTop()
{
	FVector Start = GetActorLocation();
	Start.Z += GameConstants::TileSize * 2;
	SetActorLocation(Start);
}

void ASplash::SpawnBottom()
{
	FVector Start = GetActorLocation();
	SetActorLocation(Start);
}



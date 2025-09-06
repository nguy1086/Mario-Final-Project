// Fill out your copyright notice in the Description page of Project Settings.


#include "Blood.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "GameConstants.h"

// Sets default values
ABlood::ABlood() :
	LifeTime(0.8f),
	Start(0.0f),
	PoolFlipbook(nullptr),
	DestroyBlood(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("PickupFlipbookComponent");
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	Tags.Add("Blood");
}

// Called when the game starts or when spawned
void ABlood::BeginPlay()
{
	Super::BeginPlay();

	FlipbookComponent->PlayFromStart();
}

// Called every frame
void ABlood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime -= DeltaTime;
	if (LifeTime < 0.0f)
	{
		if (DestroyBlood == true)
		{
			Destroy();
		}
			
		UPaperFlipbook* flipbook = nullptr;
		flipbook = PoolFlipbook;
		FlipbookComponent->SetFlipbook(flipbook);
		FlipbookComponent->PlayFromStart();
	}
}

void ABlood::Spawn(bool destroy)
{
	Start = GetActorLocation();
	Start.Y += 1.5f;
	SetActorLocation(Start);
	if (destroy)
		DestroyBlood = true;
}
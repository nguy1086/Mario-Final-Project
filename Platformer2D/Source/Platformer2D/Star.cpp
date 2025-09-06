// Fill out your copyright notice in the Description page of Project Settings.


#include "Star.h"
#include "GameConstants.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AStar::AStar() :
	Velocity(),
	LifeTime(0.0f),
	Target(),
	Start(),
	Direction()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("StarComponent");
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionProfileName("NoCollision");
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->BodyInstance.bLockXRotation = true;
	BoxComponent->BodyInstance.bLockYRotation = true;
	BoxComponent->BodyInstance.bLockZRotation = true;
	BoxComponent->BodyInstance.bLockYTranslation = true;
	RootComponent = BoxComponent;

	// Create the Flipbook component and attach it to the root component
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("StarFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AStar::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStar::Spawn(FVector target)
{
	Start = GetActorLocation();
	Target = target + GetActorLocation();
	Direction = Target - Start;
	Direction.Normalize();
	LifeTime = GameConstants::StarTimer;
	Velocity = FVector(GameConstants::StarSpeed, 0, GameConstants::StarSpeed);
}

// Called every frame
void AStar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector location = GetActorLocation();
	location += Direction * Velocity * DeltaTime;
	SetActorLocation(location);

	LifeTime -= DeltaTime;
	if (LifeTime < 0)
	{
		Destroy();
	}
}


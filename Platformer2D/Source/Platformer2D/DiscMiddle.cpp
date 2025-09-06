// Fill out your copyright notice in the Description page of Project Settings.


#include "DiscMiddle.h"
#include "Disc.h"
#include "Enemy.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ADiscMiddle::ADiscMiddle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("DiscComponent");
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetSimulatePhysics(false);
	RootComponent = BoxComponent;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("DiscFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	Tags.Add("World");
}

// Called when the game starts or when spawned
void ADiscMiddle::BeginPlay()
{
	Super::BeginPlay();
	
	FVector location = GetActorLocation();
	ADisc* disc = GetWorld()->SpawnActor<ADisc>(DiscTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
	disc->Spawn(Radius, location);
}

// Called every frame
void ADiscMiddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


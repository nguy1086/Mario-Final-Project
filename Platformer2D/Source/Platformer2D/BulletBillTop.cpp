// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBillTop.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABulletBillTop::ABulletBillTop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BulletBillTopComponent");
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->BodyInstance.bLockXRotation = true;
	BoxComponent->BodyInstance.bLockYRotation = true;
	BoxComponent->BodyInstance.bLockZRotation = true;
	BoxComponent->BodyInstance.bLockXTranslation = true;
	BoxComponent->BodyInstance.bLockYTranslation = true;
	BoxComponent->BodyInstance.bLockZTranslation = true;
	RootComponent = BoxComponent;

	// Create the Flipbook component and attach it to the root component
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("EnemyFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	Tags.Add("World");
}

// Called when the game starts or when spawned
void ABulletBillTop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletBillTop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


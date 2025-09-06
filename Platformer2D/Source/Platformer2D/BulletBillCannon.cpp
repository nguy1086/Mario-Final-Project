// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBillCannon.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "EnemyConstants.h"
#include "GameConstants.h"
#include "BulletBill.h"
#include "MarioCharacter.h"
#include "BulletBillTop.h"
#include "BulletBillBottom.h"

// Sets default values
ABulletBillCannon::ABulletBillCannon() :
	Spawn(0.0f),
	Direction(0.0f),
	CannonPosition()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BulletBillCannonBoxComponent");
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
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("BulletBillCannonFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	Tags.Add("World");
}

// Called when the game starts or when spawned
void ABulletBillCannon::BeginPlay()
{
	Super::BeginPlay();
	Spawn = EnemyConstants::BulletBillCooldown;



	CannonPosition = GetActorLocation();
	CannonPosition.Z -= GameConstants::TileSize;
	ABulletBillTop* bulletbilltop = GetWorld()->SpawnActor<ABulletBillTop>(BulletBillTopTemplate, CannonPosition, FRotator(0.0f, 0.0f, 0.0f));

	do
	{
		CannonPosition.Z -= GameConstants::TileSize;
		ABulletBillBottom* bulletbillbottom = GetWorld()->SpawnActor<ABulletBillBottom>(BulletBillBottomTemplate, CannonPosition, FRotator(0.0f, 0.0f, 0.0f));
	} while (CannonPosition.Z >= 0);
}

// Called every frame
void ABulletBillCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector location = GetActorLocation();
	AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();
	FVector mariolocation = mario->GetActorLocation();
	if (mariolocation.X < location.X)
	{
		Direction = -1.0f;
	}
	else if (mariolocation.X >= location.X)
	{
		Direction = 1.0f;
	}
	if (GetActorLocation().X > mario->GetActorLocation().X - (GameConstants::ResolutionWidth) &&
		GetActorLocation().X < mario->GetActorLocation().X + (GameConstants::ResolutionWidth))
	{
		Spawn -= DeltaTime;
		if (Spawn < 0.0f)
		{
			ABulletBill* bulletbill = GetWorld()->SpawnActor<ABulletBill>(BulletBillTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
			bulletbill->Spawn(Direction);

			Spawn = EnemyConstants::BulletBillCooldown;
		}
	}


}


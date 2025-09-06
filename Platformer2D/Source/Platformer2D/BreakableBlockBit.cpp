// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableBlockBit.h"
#include "GameConstants.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
ABreakableBlockBit::ABreakableBlockBit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USphereComponent>("Root");
	Root->SetVisibility(false);
	Root->SetCollisionProfileName("NoCollision");
	Root->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Root->SetSimulatePhysics(true);
	Root->BodyInstance.bLockXRotation = true;
	Root->BodyInstance.bLockZRotation = true;
	Root->BodyInstance.bLockYTranslation = true;
	RootComponent = Root;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("BreakableBlockBitSprite");
	SpriteComponent->SetupAttachment(Root);

	SetActorEnableCollision(false);
}

// Called every frame
void ABreakableBlockBit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().Z < GameConstants::DestroyActorAtZ)
	{
		Destroy();
	}
}

void ABreakableBlockBit::Spawn(EBreakableBlockBitType type, FVector impulse)
{
	float direction = 0.0f;

	if (type == EBreakableBlockBitType::Left)
	{
		SpriteComponent->SetSprite(LeftSprite);
		direction = -1.0f;
	}
	else if (type == EBreakableBlockBitType::Right)
	{
		SpriteComponent->SetSprite(RightSprite);
		direction = 1.0f;
	}

	Root->AddImpulse(impulse);
	Root->AddAngularImpulseInRadians(FVector(0.0f, PI * 2.0f * direction, 0.0f));
}


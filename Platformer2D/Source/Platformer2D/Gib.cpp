// Fill out your copyright notice in the Description page of Project Settings.


#include "Gib.h"
#include "GameConstants.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameConstants.h"
#include "PaperFlipbook.h"

// Sets default values
AGib::AGib()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USphereComponent>("Root");
	Root->SetVisibility(false);
	Root->SetCollisionProfileName("OverlapAll");
	Root->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Root->SetSimulatePhysics(true);
	Root->BodyInstance.bLockXRotation = true;
	Root->BodyInstance.bLockZRotation = true;
	Root->BodyInstance.bLockYTranslation = true;
	RootComponent = Root;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("BreakableBlockBitSprite");
	SpriteComponent->SetupAttachment(Root);

	//SetActorEnableCollision(false);
}

// Called every frame
void AGib::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().Z < GameConstants::DestroyActorAtZ)
	{
		Destroy();
	}
}

void AGib::Spawn(EGibType type, FVector impulse)
{
	float direction = 0.0f;

	if (type == EGibType::BoomBoomHead)
	{
		SpriteComponent->SetSprite(BoomBoomHeadSprite);
		direction = -1.0f;
	}
	else if (type == EGibType::BoomBoomArm)
	{
		SpriteComponent->SetSprite(BoomBoomArmSprite);
		direction = 1.0f;
	}
	else if (type == EGibType::ThwompEnd)
	{
		SpriteComponent->SetSprite(ThwompEndSprite);
		direction = -1.0f;
	}
	else if (type == EGibType::ThwompMiddle)
	{
		SpriteComponent->SetSprite(ThwompMiddleSprite);
		direction = 1.0f;
	}
	else if (type == EGibType::GoombaHead)
	{
		SpriteComponent->SetSprite(GoombaHeadSprite);
		direction = -1.0f;
	}
	else if (type == EGibType::GoombaFoot)
	{
		SpriteComponent->SetSprite(GoombaFootSprite);
		direction = 1.0f;
	}
	else if (type == EGibType::DryBonesHead)
	{
		SpriteComponent->SetSprite(DryBonesHeadSprite);
		direction = -1.0f;
	}
	else if (type == EGibType::Bone)
	{
		SpriteComponent->SetSprite(BoneSprite);
		direction = 1.0f;
	}

	else if (type == EGibType::ToadHead)
	{
		SpriteComponent->SetSprite(ToadHeadSprite);
		direction = -1.0f;
	}
	else if (type == EGibType::ToadArm)
	{
		SpriteComponent->SetSprite(ToadArmSprite);
		direction = 1.0f;
	}

	Root->AddImpulse(impulse);
	Root->AddAngularImpulseInRadians(FVector(0.0f, PI * 2.0f * direction, 0.0f));
}
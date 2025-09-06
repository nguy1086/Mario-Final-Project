// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinSwitch.h"
#include "PlatformerGameModeBase.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ACoinSwitch::ACoinSwitch() :
	BoxComponent(nullptr),
	FlipbookComponent(nullptr),
	AnimationComponent(nullptr),
	UsedFlipbook(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the BoxComponent and set it as the Root component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CoinSwitchBoxComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->OnComponentHit.AddDynamic(this, &ACoinSwitch::OnHit);
	RootComponent = BoxComponent;

	// Create the FlipbookComponent and attach it to the Root component
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("CoinSwitchFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	//
	AnimationComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("CoinSwitchAnimation");
	AnimationComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AnimationComponent->SetCollisionProfileName("NoCollision");
	AnimationComponent->SetLooping(false);
	AnimationComponent->SetupAttachment(RootComponent);

	// Add "CoinSwitch" to the Actor tags
	Tags.Add("CoinSwitch");
}

void ACoinSwitch::Tick(float DeltaTime)
{
	APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
	EGameState state = gameMode->State;
	if (state == EGameState::FadeOut)
	{
		Destroy();
	}
}

void ACoinSwitch::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("MarioCharacter") && Hit.Normal.Z <= -0.8f)
	{
		FlipbookComponent->SetFlipbook(UsedFlipbook);
		BoxComponent->SetCollisionProfileName("NoCollision");

		APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
		if (gameMode != nullptr)
		{
			gameMode->OnCoinSwitch();
		}
	}
}


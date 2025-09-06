// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestionCircle.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "MarioCharacter.h"
#include "PlatformerGameModeBase.h"
#include "PlatformerGameStateBase.h"
#include "GameConstants.h"
// Sets default values
AQuestionCircle::AQuestionCircle() :
	WonTimer(0.0f),
	State(EQuestionState::Unknown),
	Velocity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("QuestionCircleomponent");
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->BodyInstance.bLockYTranslation = true;
	BoxComponent->OnComponentHit.AddDynamic(this, &AQuestionCircle::OnHit);
	RootComponent = BoxComponent;

	// Create the FlipbookComponent and attach it to the Root component
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("QuestionCircleFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	Velocity.Z = 350.0f;
	Start = GetActorLocation();
	Timer = 0.75f;
}

void AQuestionCircle::Spawn()
{
	Target.Z += GameConstants::TileSize;
	Target = Start;
	State = EQuestionState::Spawning;
}

// Called when the game starts or when spawned
void AQuestionCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AQuestionCircle::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
		if (gameMode != nullptr)
		{
			gameMode->SetState(EGameState::EndGame);
			WonTimer = GameConstants::WonFadeDuration;

			Destroy();
		}
	}
}

void AQuestionCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EQuestionState::Spawning)
	{
		FVector location = GetActorLocation();
		location.Z += Velocity.Z * DeltaTime;
		SetActorLocation(location);
		Timer -= DeltaTime;
		if (Timer <= 0)
		{
			State = EQuestionState::Active;
		}
	}

}


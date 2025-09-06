// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapWall.h"
#include "GameConstants.h"
#include "MarioCharacter.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PlatformerGameModeBase.h"

// Sets default values
ATrapWall::ATrapWall() :
	Velocity(),
	State(EWallState::Unknown),
	BoxComponent(nullptr),
	FlipbookComponent(nullptr),
	CheckOverlap(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("WallComponent");
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->BodyInstance.bLockXTranslation = true;
	RootComponent = BoxComponent;

	CheckOverlap = CreateDefaultSubobject<UBoxComponent>("WallOverlap");
	CheckOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CheckOverlap->SetCollisionProfileName("OverlapAll");
	CheckOverlap->SetGenerateOverlapEvents(true);
	CheckOverlap->OnComponentBeginOverlap.AddDynamic(this, &ATrapWall::Overlap);
	CheckOverlap->SetupAttachment(RootComponent);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("WallFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	Tags.Add("World");
}

// Called when the game starts or when spawned
void ATrapWall::BeginPlay()
{
	Super::BeginPlay();
	OriginalLocation = GetActorLocation();

	SetState(EWallState::Active);
}

void ATrapWall::Overlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("World") && State == EWallState::Dropping)
	{
		SetState(EWallState::Inactive);
	}
	else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
	{
		Destroy();
	}
}

// Called every frame
void ATrapWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (State == EWallState::Active)
	{
		AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();

		if (mario->GetActorLocation().X > GetActorLocation().X + 128.0f)
		{
			SetState(EWallState::Dropping);
		}
	}
	else if (State == EWallState::Dropping)
	{
		FVector loc = GetActorLocation();
		loc.Z += Velocity.Z * DeltaTime;
		SetActorLocation(loc);
		if (loc.Z < (OriginalLocation.Z - 128.0f))
		{
			SetState(EWallState::Inactive);
		}
	}
	else if (State == EWallState::Inactive)
	{
		APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
		EGameState state = gameMode->State;
		if (state == EGameState::FadeOut)
		{
			SetState(EWallState::Active);
			SetActorLocation(OriginalLocation);
		}
	}
}

void ATrapWall::SetState(EWallState state)
{
	if (State != state)
	{
		State = state;
		if (State == EWallState::Active)
		{

		}
		else if (State == EWallState::Dropping)
		{
			Velocity.Z = -GameConstants::WallSpeed;
		}
		else if (State == EWallState::Inactive)
		{
			APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
			gameMode->ApplyCameraShake();
			Velocity = (FVector(0, 0, 0));
		}
	}
}


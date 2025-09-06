// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketProjectile.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "MarioCharacter.h"
#include "MarioPlayerState.h"
#include "GameConstants.h"
#include "Explosion.h"

// Sets default values
ARocketProjectile::ARocketProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BoxComponent = CreateDefaultSubobject<UBoxComponent>("RocketComponent");
	//BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//BoxComponent->SetCollisionProfileName("Fireball");
	//BoxComponent->SetNotifyRigidBodyCollision(true);
	//BoxComponent->SetSimulatePhysics(false);
	//BoxComponent->OnComponentHit.AddDynamic(this, &ARocketProjectile::OnHit);
	//BoxComponent->BodyInstance.bLockXRotation = true;
	//BoxComponent->BodyInstance.bLockYRotation = true;
	//BoxComponent->BodyInstance.bLockZRotation = true;
	//BoxComponent->BodyInstance.bLockYTranslation = true;
	//RootComponent = BoxComponent;

	CheckOverlap = CreateDefaultSubobject<UBoxComponent>("RocketCheckOverlap");
	CheckOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CheckOverlap->SetCollisionProfileName("OverlapAll");
	CheckOverlap->SetGenerateOverlapEvents(true);
	CheckOverlap->OnComponentBeginOverlap.AddDynamic(this, &ARocketProjectile::OnCheckOverlap);
	RootComponent = CheckOverlap;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("RocketFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	Tags.Add("Rocket");
}

void ARocketProjectile::Spawn()
{
	AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();
	PlayerState = mario->GetPlayerState<AMarioPlayerState>();
	EMarioState state = PlayerState->State;
	EMarioDirection marioDirection = PlayerState->Direction;
	FVector marioloc = mario->GetActorLocation();

	if (marioDirection == EMarioDirection::Left && state != EMarioState::Ducking)
	{
		Velocity.X = -GameConstants::RocketSpeed;
		FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		SetActorLocation(FVector(marioloc.X - 32.0f, 0.0f, marioloc.Z + 8.0f));
	}

	else if (marioDirection == EMarioDirection::Right && state != EMarioState::Ducking)
	{
		Velocity.X = GameConstants::RocketSpeed;
		SetActorLocation(FVector(marioloc.X + 32.0f, 0.0f, marioloc.Z + 8.0f));
	}

	else if (state == EMarioState::Ducking)
	{
		Velocity.Z = GameConstants::RocketSpeed;
		FlipbookComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		SetActorLocation(FVector(marioloc.X, 0.0f, marioloc.Z + 32.0f));
	}
}

// Called when the game starts or when spawned
void ARocketProjectile::BeginPlay()
{
	Super::BeginPlay();
	LifeTime = GameConstants::FireballLifeTime;
}

// Called every frame
void ARocketProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector location = GetActorLocation();
	location += Velocity * DeltaTime;
	SetActorLocation(location);

	LifeTime -= DeltaTime;
	if (LifeTime < 0.0f)
	{
		AExplosion* explosion = GetWorld()->SpawnActor<AExplosion>(ExplosionTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
		explosion->Spawn();
		Destroy();
	}
}

void ARocketProjectile::OnCheckOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy") || OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("ItemBox") || OtherActor->ActorHasTag("BreakableBlock"))
	{
		LifeTime = 0.01f;
	}
}

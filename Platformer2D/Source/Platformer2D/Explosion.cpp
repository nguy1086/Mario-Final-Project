// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "GameConstants.h"
#include "Components/BoxComponent.h"
#include "PlatformerGameModeBase.h"
#include "Kismet/GameplayStatics.h" 
#include "MarioCharacter.h"
// Sets default values
AExplosion::AExplosion() :
	LifeTime(1.0f),
	Start(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("ExplosionBoxComponent");
	BoxComponent->SetCollisionProfileName("OverlapAll");
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AExplosion::Overlap);
	RootComponent = BoxComponent;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("PickupFlipbookComponent");
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	CheckOverlap = CreateDefaultSubobject<UBoxComponent>("ExplosionOverlap");
	CheckOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CheckOverlap->SetCollisionProfileName("OverlapAll");
	CheckOverlap->SetGenerateOverlapEvents(true);
	CheckOverlap->OnComponentBeginOverlap.AddDynamic(this, &AExplosion::Overlap);
	CheckOverlap->SetupAttachment(RootComponent);

	Tags.Add("Explosion");
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
	
	if (ExplosionSound != nullptr)
		UGameplayStatics::PrimeSound(ExplosionSound);

	FlipbookComponent->PlayFromStart();
}

// Called every frame
void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Super::Tick(DeltaTime);
	LifeTime -= DeltaTime;

	if (LifeTime < 0.0f)
	{
		Destroy();
	}
}

void AExplosion::Spawn()
{
	Start = GetActorLocation();
	Start.Y += 1.5f;
	SetActorLocation(Start);

	if (ExplosionSound != nullptr)
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, Start);

	APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
	gameMode->ApplyCameraShake();
}

void AExplosion::Overlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		Cast<AMarioCharacter>(OtherActor)->HandleDamage();
	}
}
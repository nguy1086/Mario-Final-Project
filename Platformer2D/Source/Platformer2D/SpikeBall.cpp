// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeBall.h"
#include "EnemyConstants.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "MarioCharacter.h"
#include "GameConstants.h"
ASpikeBall::ASpikeBall() :
    Velocity(),
    State(ESpikeBallState::Unknown),
    CollisionCheck(nullptr),
    SpawningDuration(0.0f),
    AimmingDuration(0.0f),
    Turn(0.0f),
    LifeTime(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionProfileName("OverlapAll");
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetNotifyRigidBodyCollision(true);
    BoxComponent->OnComponentHit.AddDynamic(this, &ASpikeBall::OnHit);

    CollisionCheck = CreateDefaultSubobject<UBoxComponent>("SpikeBallCheckOverlap");
    CollisionCheck->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CollisionCheck->SetCollisionProfileName("OverlapAll");
    CollisionCheck->SetGenerateOverlapEvents(true);
    CollisionCheck->OnComponentBeginOverlap.AddDynamic(this, &ASpikeBall::OnCheckOverlap);
    CollisionCheck->SetupAttachment(RootComponent);

    Tags.Add("SpikeBall");
}


void ASpikeBall::BeginPlay()
{
	Super::BeginPlay();


}



// Called every frame
void ASpikeBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (State == ESpikeBallState::Spawning)
    {
        if (SpawningDuration > 0.0f)
        {
            SpawningDuration -= DeltaTime;

            float pct = 1.0f - (SpawningDuration / EnemyConstants::SpikeReloading);
            FVector location = FMath::Lerp(Start, Target, pct);
            SetActorLocation(location);

            if (SpawningDuration <= 0.0f)
            {
                SpawningDuration = 0.0f;
                SetActorLocation(Target);
                SetState(ESpikeBallState::Aimming);
            }
        }
    }
    else if (State == ESpikeBallState::Aimming)
    {
        if (AimmingDuration > 0.0f)
        {
            AimmingDuration -= DeltaTime;

            float pct = 1.0f - (AimmingDuration / EnemyConstants::SpikeAimming);
            FVector location = FMath::Lerp(Start, Target, pct);
            SetActorLocation(location);

            if (AimmingDuration <= 0.0f)
            {
                AimmingDuration = 0.0f;
                SetActorLocation(Target);
                SetState(ESpikeBallState::Active);
            }
        }
    }
    else if (State == ESpikeBallState::Active)
    {
        FVector ballLoc = GetActorLocation();
        ballLoc += Velocity * DeltaTime;
        SetActorLocation(ballLoc);
    }
}

void ASpikeBall::Spawn(float direction)
{
    Start = GetActorLocation();
    Start.Y -= 1.0f;
    SetActorLocation(Start);
    Turn = direction;
    SetState(ESpikeBallState::Spawning);
    Start = GetActorLocation();
    Target = Start;
    Target.Z += GameConstants::TileSize;
    if (Turn > 0)
        FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
    else if (Turn < 0)
        FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));


    SetActorLocation(Start);
}

void ASpikeBall::SetState(ESpikeBallState state)
{
    if (State != state)
    {
        State = state;
        if (State == ESpikeBallState::Spawning)
        {
            SpawningDuration = EnemyConstants::SpikeReloading;
        }
        else if (State == ESpikeBallState::Aimming)
        {
            Start = GetActorLocation();
            if (Turn > 0)
                Target.X -= GameConstants::TileHalfSize;
            else if (Turn < 0)
                Target.X += GameConstants::TileHalfSize;

            AimmingDuration = EnemyConstants::SpikeAimming;
        }
        else if (State == ESpikeBallState::Active)
        {
            if (Turn > 0)
                Velocity.X = EnemyConstants::SpikeBallSpeed;
            else if (Turn < 0)
                Velocity.X = -EnemyConstants::SpikeBallSpeed;
        }

    }
    UpdateFlipbook();
}

void ASpikeBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor->ActorHasTag("MarioCharacter"))
    {
        Cast<AMarioCharacter>(OtherActor)->HandleDamage();
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        Destroy();
    }
}

void ASpikeBall::OnCheckOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("MarioCharacter"))
    {
        Cast<AMarioCharacter>(OtherActor)->HandleDamage();
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        Destroy();
    }
}

void ASpikeBall::UpdateFlipbook()
{
    UPaperFlipbook* flipbook = nullptr;

    if (State == ESpikeBallState::Spawning || State == ESpikeBallState::Aimming)
    {
        flipbook = IdleFlipbook;

    }
    else if (State == ESpikeBallState::Active)
    {
        flipbook = ActiveFlipbook;
    }

    FlipbookComponent->SetFlipbook(flipbook);
    FlipbookComponent->PlayFromStart();
}


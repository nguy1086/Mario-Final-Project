// Fill out your copyright notice in the Description page of Project Settings.


#include "Boo.h"
#include "MarioPlayerState.h"
#include "MarioCharacter.h"
#include "EnemyConstants.h"
#include "Components/BoxComponent.h"
#include "MarioController.h"
#include "PaperFlipbookComponent.h"
#include "GameConstants.h"

ABoo::ABoo() :
    State(EBooState::Unknown),
    Direction(EBooDirection::Unknown),
    Velocity(),
    PlayerState(nullptr),
    InactiveFlipbook(nullptr),
    ActiveFlipbook(nullptr),
    Shotted(false)
{
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionProfileName("EnemyDynamic");
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetNotifyRigidBodyCollision(true);
    BoxComponent->OnComponentHit.AddDynamic(this, &ABoo::OnHit);

    EnemyCollisionCheck = CreateDefaultSubobject<UBoxComponent>("BooCheckOverlap");
    EnemyCollisionCheck->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EnemyCollisionCheck->SetCollisionProfileName("OverlapAll");
    EnemyCollisionCheck->SetGenerateOverlapEvents(true);
    EnemyCollisionCheck->OnComponentBeginOverlap.AddDynamic(this, &ABoo::OnEnemyCollision);
    EnemyCollisionCheck->SetupAttachment(RootComponent);

    Type = EEnemyType::Boo;
    Tags.Add("Boo");
}

void ABoo::BeginPlay()
{
    Super::BeginPlay();

    SetState(EBooState::Active);
}

void ABoo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();

    PlayerState = mario->GetPlayerState<AMarioPlayerState>();

    EMarioDirection marioDirection = PlayerState->Direction;
    if (!Shotted || State != EBooState::Shotted)
    {
        if ((marioDirection == EMarioDirection::Left && Direction == EBooDirection::Right)
            || (marioDirection == EMarioDirection::Right && Direction == EBooDirection::Left))
        {
            SetState(EBooState::Inactive);
        }
        else
        {
            SetState(EBooState::Active);
        }
    }

    //only move on screen
    if (GetActorLocation().X > mario->GetActorLocation().X - (GameConstants::ResolutionWidth / 2.0f) &&
        GetActorLocation().X < mario->GetActorLocation().X + (GameConstants::ResolutionWidth / 2.0f))
    {
        if (State == EBooState::Active)
        {
            Velocity = FVector(EnemyConstants::BooSpeed, EnemyConstants::BooSpeed, EnemyConstants::BooSpeed);

            FVector start = GetActorLocation();
            FVector direction = mario->GetActorLocation() - start;

            direction.Normalize();

            start += direction * Velocity * DeltaTime;

            if (start.X > mario->GetActorLocation().X)
            {
                SetDirection(EBooDirection::Left);
                SetActorRotation(FRotator(0, 0, 0));
            }
            else if (start.X < mario->GetActorLocation().X)
            {
                SetDirection(EBooDirection::Right);
                SetActorRotation(FRotator(0, 180, 0));
            }

            SetActorLocation(start);
        }
        else if (State == EBooState::Inactive)
        {
            FVector start = GetActorLocation();

            if (start.X > mario->GetActorLocation().X)
            {
                SetDirection(EBooDirection::Left);
                SetActorRotation(FRotator(0, 0, 0));
            }
            else if (start.X < mario->GetActorLocation().X)
            {
                SetDirection(EBooDirection::Right);
                SetActorRotation(FRotator(0, 180, 0));
            }
        }
        else if (State == EBooState::Shotted)
        {
            FVector location = GetActorLocation();
            location.Z += Velocity.Z * DeltaTime;
            SetActorLocation(location);
            FlipbookComponent->SetRelativeRotation(FRotator(Velocity.Z, 0.0f, 0.0f));
            if (GetActorLocation().Z < 0.0f)
            {
                Destroy();
            }
            else
            {
                Velocity.Z -= 550.0f * DeltaTime;
            }
        }
    }
}

void ABoo::SetState(EBooState state)
{
    if (State != state)
    {
        State = state;
        if (State == EBooState::Active)
        {
        }
        else if (State == EBooState::Inactive)
        {
        }
        else if (State == EBooState::Shotted)
        {
            //Velocity.Z = -EnemyConstants::BulletBillSpeed;
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            FlipbookComponent->SetCollisionProfileName("NoCollision");
            BoxComponent->SetSimulatePhysics(true);
            Shotted = true;
        }
    }
    UpdateFlipbook();
}

void ABoo::SetDirection(EBooDirection dir)
{
    if (Direction != dir)
    {
        Direction = dir;
        if (Direction == EBooDirection::Left)
        {
        }
        else if (Direction == EBooDirection::Right)
        {
        }
    }
    UpdateFlipbook();
}

void ABoo::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor->ActorHasTag("MarioCharacter"))
    {
        Cast<AMarioCharacter>(OtherActor)->HandleDamage();
    }
    else if (OtherActor->ActorHasTag("Bullet"))
    {
        SetState(EBooState::Shotted);
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        Destroy();
    }
}

void ABoo::OnEnemyCollision(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Enemy") || OtherActor->ActorHasTag("World"))
    {
        BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    else
    {
        BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

    if (OtherActor->ActorHasTag("MarioCharacter"))
    {
        Cast<AMarioCharacter>(OtherActor)->HandleDamage();
    }
    else if (OtherActor->ActorHasTag("Bullet"))
    {
        SetState(EBooState::Shotted);
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        Destroy();
    }
}

void ABoo::UpdateFlipbook()
{
    UPaperFlipbook* flipbook = nullptr;

    if (State == EBooState::Inactive || State == EBooState::Shotted)
    {
        flipbook = InactiveFlipbook;
    }
    else if (State == EBooState::Active || State == EBooState::Shotted)
    {
        flipbook = ActiveFlipbook;
    }

    FlipbookComponent->SetFlipbook(flipbook);
    FlipbookComponent->PlayFromStart();
}
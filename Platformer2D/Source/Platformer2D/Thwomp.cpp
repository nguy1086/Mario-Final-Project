// Fill out your copyright notice in the Description page of Project Settings.


#include "Thwomp.h"
#include "MarioPlayerState.h"
#include "MarioCharacter.h"
#include "EnemyConstants.h"
#include "Components/BoxComponent.h"
#include "MarioController.h"
#include "PaperFlipbookComponent.h"
#include "PlatformerGameModeBase.h"
#include "Kismet/GameplayStatics.h" 
#include "Gib.h"
#include "Blood.h"
AThwomp::AThwomp() :
    Delay(0.0f),
    Velocity(),
    OriginalLocation(),
    LandedLocation(),
    State(EThwompState::Unknown),
    WorldCollisionCheck(nullptr)
{
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionProfileName("EnemyDynamic");
    BoxComponent->SetSimulatePhysics(true);
    BoxComponent->SetNotifyRigidBodyCollision(true);
    BoxComponent->OnComponentHit.AddDynamic(this, &AThwomp::OnHit);
    BoxComponent->SetEnableGravity(false);
    BoxComponent->BodyInstance.bLockXTranslation = true;

    WorldCollisionCheck = CreateDefaultSubobject<UBoxComponent>("ThwompOverlap");
    WorldCollisionCheck->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    WorldCollisionCheck->SetCollisionProfileName("OverlapAll");
    WorldCollisionCheck->SetGenerateOverlapEvents(true);
    WorldCollisionCheck->OnComponentBeginOverlap.AddDynamic(this, &AThwomp::OnWorldCollision);
    WorldCollisionCheck->SetupAttachment(RootComponent);

    Type = EEnemyType::Thwomp;
    Tags.Add("Thwomp");
}

void AThwomp::BeginPlay()
{
    Super::BeginPlay();

    if (ThwompSound != nullptr)
        UGameplayStatics::PrimeSound(ThwompSound);

    OriginalLocation = GetActorLocation();
    SetState(EThwompState::Active);
}

void AThwomp::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();
    APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();

    if (State == EThwompState::Active)
    {
        FVector marioLoc = mario->GetActorLocation();
        FVector thwompLoc = GetActorLocation();
        if (marioLoc.X >= (thwompLoc.X - EnemyConstants::ThwompRange) &&
            marioLoc.X <= (thwompLoc.X + EnemyConstants::ThwompRange))
        {
            SetState(EThwompState::Dropping);
        }
        else
        {
            SetActorLocation(OriginalLocation);
        }
    }
    else if (State == EThwompState::Dropping)
    {
        FVector thwompLoc = GetActorLocation();
        thwompLoc.Z += Velocity.Z * DeltaTime;
        SetActorLocation(thwompLoc);
    }
    else if (State == EThwompState::Delay)
    {
        SetActorLocation(LandedLocation);
        Delay -= DeltaTime;
        if (Delay < 0.0f)
        {
            Delay = 0.0f;
            SetState(EThwompState::Raise);
        }
    }
    else if (State == EThwompState::Raise)
    {
        FVector thwompLoc = GetActorLocation();
        thwompLoc.Z += Velocity.Z * DeltaTime;
        SetActorLocation(thwompLoc);
        if (thwompLoc.Z >= OriginalLocation.Z)
        {
            SetActorLocation(OriginalLocation);
            SetState(EThwompState::Active);
        }
    }
    else if (State == EThwompState::Shotted)
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
    else if (State == EThwompState::Exploded)
    {

    }
}

void AThwomp::SetState(EThwompState state)
{
    if (State != state)
    {
        State = state;
        if (State == EThwompState::Active)
        {
            Velocity.Z = EnemyConstants::ThwompDropSpeed;
        }
        else if (State == EThwompState::Dropping)
        {
            Velocity.Z = EnemyConstants::ThwompDropSpeed;
        }
        else if (State == EThwompState::Delay)
        {
            Velocity.Z = EnemyConstants::ThwompRaiseSpeed;
            Delay = EnemyConstants::ThwompDelayTimer;
        }
        else if (State == EThwompState::Raise)
        {
            Velocity.Z = EnemyConstants::ThwompRaiseSpeed;
        }
        else if (State == EThwompState::Shotted)
        {
            Velocity.Z = -EnemyConstants::BulletBillSpeed;
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            FlipbookComponent->SetCollisionProfileName("NoCollision");
            BoxComponent->SetSimulatePhysics(true);
        }
        else if (State == EThwompState::Exploded)
        {
            FVector location = GetActorLocation();
            const int32 kNumBits = 4;
            AGib* bits[kNumBits] = {};
            FVector locations[kNumBits] = { location + FVector(-8.0f, 0.0f, 8.0f),
                                            location + FVector(-8.0f, 0.0f, -8.0f),
                                            location + FVector(8.0f, 0.0f, 8.0f),
                                            location + FVector(8.0f, 0.0f, -8.0f) };

            FVector impulses[kNumBits] = { FVector(-150.0f, 0.0f, 350.0f),
                                            FVector(-150.0f, 0.0f, 300.0f),
                                            FVector(150.0f, 0.0f, 350.0f),
                                            FVector(150.0f, 0.0f, 300.0f) };

            EGibType types[kNumBits] = { EGibType::ThwompEnd,
                                                        EGibType::ThwompEnd,
                                                        EGibType::ThwompMiddle,
                                                        EGibType::ThwompMiddle };

            for (int32 i = 0; i < kNumBits; i++)
            {
                bits[i] = GetWorld()->SpawnActor<AGib>(GibTemplate, locations[i], FRotator(0.0f, 0.0f, 0.0f));
                bits[i]->Spawn(types[i], impulses[i]);
            }
            ABlood* blood = GetWorld()->SpawnActor<ABlood>(BloodTemplate, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
            blood->Spawn(true);

            Destroy();
        }
    }
}

void AThwomp::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if ((OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("BreakableBlock")) && 
        (State != EThwompState::Raise && State != EThwompState::Shotted && State != EThwompState::Exploded))
    {
        APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
        if (ThwompSound != nullptr)
            UGameplayStatics::PlaySoundAtLocation(this, ThwompSound, GetActorLocation());

        gameMode->ApplyCameraShake();
        LandedLocation = GetActorLocation();
        SetState(EThwompState::Delay);
    }
    else if (OtherActor->ActorHasTag("MarioCharacter"))
    {
        Cast<AMarioCharacter>(OtherActor)->HandleDamage();
        SetState(EThwompState::Raise);
    }
    else if (OtherActor->ActorHasTag("Bullet"))
    {
        SetState(EThwompState::Shotted);
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        SetState(EThwompState::Exploded);
    }
}

void AThwomp::OnWorldCollision(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if ((OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("BreakableBlock")) && 
        (State != EThwompState::Raise && State != EThwompState::Shotted && State != EThwompState::Exploded))
    {
        APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
        if (ThwompSound != nullptr)
            UGameplayStatics::PlaySoundAtLocation(this, ThwompSound, GetActorLocation());

        gameMode->ApplyCameraShake();
        LandedLocation = GetActorLocation();
        SetState(EThwompState::Delay);
    }
    else if (OtherActor->ActorHasTag("MarioCharacter"))
    {
        Cast<AMarioCharacter>(OtherActor)->HandleDamage();
        SetState(EThwompState::Raise);
    }
    else if (OtherActor->ActorHasTag("Bullet"))
    {
        SetState(EThwompState::Shotted);
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        SetState(EThwompState::Exploded);
    }
}



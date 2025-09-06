// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomBoom.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "EnemyConstants.h"
#include "MarioCharacter.h"
#include "QuestionCircle.h"
#include "PlatformerGameModeBase.h"
#include "Star.h"
#include "Gib.h"
#include "Blood.h"

ABoomBoom::ABoomBoom() :
    Velocity(),
    IdleDuration(0.0f),
    WalkDuration(0.0f),
    HurtDuration(0.0f),
    DeadDuration(0.0f),
    State(EBoomBoomState::Unknown),
    Lives(0),
    FireballHits(0),
    WalkSwapDuration(0.0f),
    JumpDelay(0.0f),
    SpeedIncrement(1)
{

    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionProfileName("EnemyDynamic");
    BoxComponent->SetSimulatePhysics(true);
    BoxComponent->SetNotifyRigidBodyCollision(true);
    BoxComponent->OnComponentHit.AddDynamic(this, &ABoomBoom::OnHit);

    FrontOverlap = CreateDefaultSubobject<UBoxComponent>("BoomBoomFrontOverlap");
    FrontOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    FrontOverlap->SetCollisionProfileName("OverlapAll");
    FrontOverlap->SetGenerateOverlapEvents(true);
    FrontOverlap->OnComponentBeginOverlap.AddDynamic(this, &ABoomBoom::OnForwardOverlap);
    FrontOverlap->SetupAttachment(RootComponent);

    BackOverlap = CreateDefaultSubobject<UBoxComponent>("BoomBoomBackOverlap");
    BackOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BackOverlap->SetCollisionProfileName("OverlapAll");
    BackOverlap->SetGenerateOverlapEvents(true);
    BackOverlap->OnComponentBeginOverlap.AddDynamic(this, &ABoomBoom::OnBackwardOverlap);
    BackOverlap->SetupAttachment(RootComponent);

    BottomOverlap = CreateDefaultSubobject<UBoxComponent>("BoomBoomBottomOverlap");
    BottomOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BottomOverlap->SetCollisionProfileName("OverlapAll");
    BottomOverlap->SetGenerateOverlapEvents(true);
    BottomOverlap->OnComponentBeginOverlap.AddDynamic(this, &ABoomBoom::OnBottomOverlap);
    BottomOverlap->SetupAttachment(RootComponent);

    Type = EEnemyType::BoomBoom;
    Tags.Add("BoomBoom");
}

void ABoomBoom::BeginPlay()
{
    Super::BeginPlay();

    Lives = EnemyConstants::BoomBoomLives;
    SetState(EBoomBoomState::Dormant);
}

void ABoomBoom::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (State == EBoomBoomState::Dormant)
    {
        AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();

        FVector marioloc = mario->GetActorLocation();

        if (marioloc.X >= GetActorLocation().X - 256.0f)
        {
            SetState(EBoomBoomState::Walking);
        }
    }
    else if (State == EBoomBoomState::Walking)
    {
        FVector location = GetActorLocation();
        location.X += (Velocity.X * DeltaTime) * (SpeedIncrement);
        SetActorLocation(location);

        WalkSwapDuration -= DeltaTime;
        if (WalkSwapDuration < 0.0f)
        {
            WalkSwapDuration = FMath::RandRange(EnemyConstants::BoomBoomSwapWalkMin, EnemyConstants::BoomBoomSwapWalkMax);
            if (Velocity.X < 0.0f)
                Velocity.X = EnemyConstants::BoomBoomSpeed;
            else if (Velocity.X > 0.0f)
                Velocity.X = -EnemyConstants::BoomBoomSpeed;
        }

        WalkDuration -= DeltaTime;
        if (WalkDuration < 0.0f)
        {
            WalkDuration = 0.0f;
            int32 idleorjump = FMath::RandRange(0, 9) % 2;
            float speeds[] = { 0, 1 };
            int choice = speeds[idleorjump];

            if (choice == 0)
                SetState(EBoomBoomState::Idle);
            else
                SetState(EBoomBoomState::Jump);
        }
    }
    else if (State == EBoomBoomState::Idle)
    {
        IdleDuration -= DeltaTime;
        if (IdleDuration < 0.0f)
        {
            IdleDuration = 0.0f;
            SetState(EBoomBoomState::Walking);
        }
    }
    else if (State == EBoomBoomState::Hurt)
    {
        HurtDuration -= DeltaTime;
        if (HurtDuration < 0.0f)
        {
            HurtDuration = 0.0f;
            SetState(EBoomBoomState::Idle);
        }
    }
    else if (State == EBoomBoomState::Dead)
    {
        DeadDuration -= DeltaTime;

        if (DeadDuration < 0.0f)
        {
            SpawnQuestionMark();
        }
    }
    else if (State == EBoomBoomState::Jump)
    {
        JumpDelay -= DeltaTime;
        if (JumpDelay <= 0.0f)
        {
         
            FVector location = GetActorLocation();
            location += Velocity * DeltaTime;
            SetActorLocation(location);
            //if (location.Z >= EnemyConstants::BoomBoomJumpApex)
            //{
            //    JumpDelay = 0.0f;
            //    SetState(EBoomBoomState::Falling);
            //}
            if (Velocity.Z <= 100)
            {
                Velocity.Z = 0;
                SetState(EBoomBoomState::Falling);
            }
            else
            {
                Velocity.Z -= 1.0f * 750.0f * DeltaTime;
            }
        }
    }
    else if (State == EBoomBoomState::Falling)
    {
        FVector location = GetActorLocation();
        location += Velocity * DeltaTime;
        SetActorLocation(location);
        if (location.Z < OriginalPosition.Z + 10.0f)
        {
            APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
            gameMode->ApplyCameraShake();
            SetActorLocation(FVector(location.X, location.Y, OriginalPosition.Z));
            SetState(EBoomBoomState::Walking);
        }
        else
        {
            Velocity.Z -= 1.0f * 950.0f * DeltaTime;
        }
    }
}

void ABoomBoom::SetState(EBoomBoomState state)
{
    if (State != state)
    {
        State = state;

        if (State == EBoomBoomState::Walking)
        {
            BoxComponent->SetSimulatePhysics(true);
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            BoxComponent->SetCollisionProfileName("EnemyDynamic");
            ApplyInitialVelocity();
            WalkSwapDuration = FMath::RandRange(EnemyConstants::BoomBoomSwapWalkMin, EnemyConstants::BoomBoomSwapWalkMax);
            WalkDuration = EnemyConstants::BoomBoomWalkingTimer;
        }
        else if (State == EBoomBoomState::Idle || State == EBoomBoomState::Dormant)
        {
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            Velocity.Z = 0.0f;
            IdleDuration = EnemyConstants::BoomBoomIdleTimer;
        }
        else if (State == EBoomBoomState::Hurt)
        {
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            Velocity.Z = 0.0f;
            HurtDuration = EnemyConstants::BoomBoomHurtTimer;
        }
        else if (State == EBoomBoomState::Dead)
        {
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            Velocity.Z = 0.0f;
            DeadDuration = EnemyConstants::BoomBoomDeadTimer;
        }
        else if (State == EBoomBoomState::Jump)
        {
            OriginalPosition = GetActorLocation();
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            Velocity.Z = EnemyConstants::BoomBoomJumpSpeed;
            JumpDelay = EnemyConstants::BoomBoomJumpDelay;
        }
        else if (State == EBoomBoomState::Falling)
        {
            BoxComponent->SetSimulatePhysics(false);
            Velocity.Z = 0;
        }
        else if (State == EBoomBoomState::Exploded)
        {
            FVector location = GetActorLocation();
            const int32 kNumBits = 3;
            AGib* bits[kNumBits] = {};
            FVector locations[kNumBits] = { location + FVector(-8.0f, 0.0f, 8.0f),
                                            location + FVector(-8.0f, 0.0f, -8.0f),
                                            location + FVector(8.0f, 0.0f, 8.0f) };

            FVector impulses[kNumBits] = { FVector(-150.0f, 0.0f, 350.0f),
                                            FVector(-150.0f, 0.0f, 300.0f),
                                            FVector(150.0f, 0.0f, 350.0f) };

            EGibType types[kNumBits] = { EGibType::BoomBoomHead,
                                                        EGibType::BoomBoomArm,
                                                        EGibType::BoomBoomArm };

            for (int32 i = 0; i < kNumBits; i++)
            {
                bits[i] = GetWorld()->SpawnActor<AGib>(GibTemplate, locations[i], FRotator(0.0f, 0.0f, 0.0f));
                bits[i]->Spawn(types[i], impulses[i]);
            }
            ABlood* blood = GetWorld()->SpawnActor<ABlood>(BloodTemplate, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
            blood->Spawn(false);

            SpawnQuestionMark();
        }

        UpdateFlipbook();
    }
}

void ABoomBoom::OnForwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("Goomba") || OtherActor->ActorHasTag("MushroomPickup") || OtherActor->ActorHasTag("DryBones") || OtherActor->ActorHasTag("Enemy"))
    {
        Velocity.X = -EnemyConstants::BoomBoomSpeed;
    }
    else if (OtherActor->ActorHasTag("Fireball") && 
        (State != EBoomBoomState::Hurt || State != EBoomBoomState::Dead ||
        State != EBoomBoomState::Dormant || State != EBoomBoomState::Idle))
    {
        FireballHits++;
        if (FireballHits >= EnemyConstants::BoomBoomFireballHits)
        {
            if (Lives > 0)
            {
                SetState(EBoomBoomState::Hurt);
                Lives--;
                SpeedIncrement += 0.5f;
                FireballHits = 0;
            }
            if (Lives <= 0)
                SetState(EBoomBoomState::Dead);

        }
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Bullet"))
    {
        SetState(EBoomBoomState::Dead);
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        SetState(EBoomBoomState::Exploded);
    }
}

void ABoomBoom::OnBackwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("Goomba") || OtherActor->ActorHasTag("MushroomPickup") || OtherActor->ActorHasTag("DryBones") || OtherActor->ActorHasTag("Enemy"))
    {
        Velocity.X = EnemyConstants::BoomBoomSpeed;
    }
    else if (OtherActor->ActorHasTag("Fireball") && 
        (State != EBoomBoomState::Hurt || State != EBoomBoomState::Dead ||
        State != EBoomBoomState::Dormant || State != EBoomBoomState::Idle))
    {
        FireballHits++;
        if (FireballHits >= EnemyConstants::BoomBoomFireballHits)
        {
            if (Lives > 0)
            {
                SetState(EBoomBoomState::Hurt);
                Lives--;
                SpeedIncrement += 0.5f;
                FireballHits = 0;
            }
            if (Lives <= 0)
                SetState(EBoomBoomState::Dead);

        }
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Bullet"))
    {
        SetState(EBoomBoomState::Dead);
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        SetState(EBoomBoomState::Exploded);
    }
}

void ABoomBoom::OnBottomOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("World") && State == EBoomBoomState::Falling)
    {
        APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
        gameMode->ApplyCameraShake();
        SetState(EBoomBoomState::Walking);
    }
}

void ABoomBoom::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor->ActorHasTag("MarioCharacter") && (State != EBoomBoomState::Hurt || State != EBoomBoomState::Dead))
    {
        if ((Hit.Normal.X <= -0.8f || Hit.Normal.X >= 0.8f))
        {
            // if hit on side
            Cast<AMarioCharacter>(OtherActor)->HandleDamage();
        }
        else if (Hit.Normal.Z >= 0.7f && (State != EBoomBoomState::Hurt || State != EBoomBoomState::Dead))
        {
            // if landed on mario
            Cast<AMarioCharacter>(OtherActor)->HandleDamage();
        }
        else if (Hit.Normal.Z <= -0.7f)
        {
            // Mario landed on
            if (State == EBoomBoomState::Walking)
            {
                if (Lives > 0)
                {
                    SetState(EBoomBoomState::Hurt);
                    Lives--;
                    SpeedIncrement += 0.5f;
                }
                if (Lives <= 0)
                    SetState(EBoomBoomState::Dead);

                Cast<AMarioCharacter>(OtherActor)->ApplyBounce();
            }
            else if (State == EBoomBoomState::Idle || State == EBoomBoomState::Dormant)
            {
                Cast<AMarioCharacter>(OtherActor)->HandleDamage();
            }
        }
    }
    else if (OtherActor->ActorHasTag("Fireball") && 
        (State != EBoomBoomState::Hurt || State != EBoomBoomState::Dead ||
            State != EBoomBoomState::Dormant || State != EBoomBoomState::Idle))
    {
        FireballHits++;
        if (FireballHits >= EnemyConstants::BoomBoomFireballHits)
        {
            if (Lives > 0)
            {
                SetState(EBoomBoomState::Hurt);
                Lives--;
                SpeedIncrement += 0.5f;
                FireballHits = 0;
            }
            if (Lives <= 0)
                SetState(EBoomBoomState::Dead);

        }
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Bullet"))
    {
        SetState(EBoomBoomState::Dead);
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        SetState(EBoomBoomState::Exploded);
    }
}

void ABoomBoom::ApplyInitialVelocity()
{
    if (Velocity == FVector(NULL, NULL, NULL))//moves at same direction no matter
    {
        if (InitialDirection == EBoomBoomDirection::Random)
        {
            int32 index = FMath::RandRange(0, 9) % 2;
            float speeds[] = { EnemyConstants::BoomBoomSpeed, -EnemyConstants::BoomBoomSpeed };
            Velocity.X = speeds[index];
        }
        else if (InitialDirection == EBoomBoomDirection::Right)
        {
            Velocity.X = -EnemyConstants::DryBonesSpeed;
        }
        else if (InitialDirection == EBoomBoomDirection::Left)
        {
            Velocity.X = EnemyConstants::DryBonesSpeed;
        }
    }
}

void ABoomBoom::UpdateFlipbook()
{
    UPaperFlipbook* flipbook = nullptr;

    if (State == EBoomBoomState::Walking || State == EBoomBoomState::Jump || State == EBoomBoomState::Falling)
    {
        flipbook = WalkingFlipbook;
        FlipbookComponent->SetRelativeLocation(FVector(0, 0, 0.0f));
    }
    else if (State == EBoomBoomState::Idle || State == EBoomBoomState::Dormant)
    {
        flipbook = IdleFlipbook;
        FlipbookComponent->SetRelativeLocation(FVector(0, 0, -6.0f));
    }
    else if (State == EBoomBoomState::Hurt)
    {
        flipbook = HurtFlipbook;
        FlipbookComponent->SetRelativeLocation(FVector(0, 0, -10.0f));
    }
    else if (State == EBoomBoomState::Dead)
    {
        flipbook = DeadFlipbook;
        FlipbookComponent->SetRelativeLocation(FVector(0, 0, -10.0f));
    }

    FlipbookComponent->SetFlipbook(flipbook);
    FlipbookComponent->PlayFromStart();
}

void ABoomBoom::SpawnQuestionMark()
{
    FVector location = GetActorLocation();
    AQuestionCircle* circle = GetWorld()->SpawnActor<AQuestionCircle>(QuestionTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
    circle->Spawn();

    //there's def a better way to write this aka a loop
    AStar* starone = GetWorld()->SpawnActor<AStar>(StarTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
    AStar* startwo = GetWorld()->SpawnActor<AStar>(StarTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
    AStar* starthree = GetWorld()->SpawnActor<AStar>(StarTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
    AStar* starfour = GetWorld()->SpawnActor<AStar>(StarTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
    AStar* starfive = GetWorld()->SpawnActor<AStar>(StarTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
    AStar* starsix = GetWorld()->SpawnActor<AStar>(StarTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
    AStar* starseven = GetWorld()->SpawnActor<AStar>(StarTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
    AStar* stareight = GetWorld()->SpawnActor<AStar>(StarTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
    starone->Spawn(FVector(0, 0, 1));
    startwo->Spawn(FVector(0, 0, -1));
    starthree->Spawn(FVector(1, 0, 0));
    starfour->Spawn(FVector(-1, 0, 0));
    starfive->Spawn(FVector(1, 0, 1));
    starsix->Spawn(FVector(1, 0, -1));
    starseven->Spawn(FVector(-1, 0, 1));
    stareight->Spawn(FVector(-1, 0, -1));

    Destroy();
}


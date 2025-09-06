// Fill out your copyright notice in the Description page of Project Settings.


#include "DryBones.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "EnemyConstants.h"
#include "MarioCharacter.h"
#include "Gib.h"

ADryBones::ADryBones() :
    InitialDirection(EDryBonesInitialDirection::Random),
    FrontOverlap(nullptr),
    BackOverlap(nullptr),
    WalkingFlipbook(nullptr),
    StunnedFlipbook(nullptr),
    Velocity(),
    StunnedDuration(0.0f),
    StirringDuration(0.0f),
    FallenDuration(0.0f)
{
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionProfileName("EnemyDynamic");
    BoxComponent->SetSimulatePhysics(true);
    BoxComponent->SetNotifyRigidBodyCollision(true);
    BoxComponent->OnComponentHit.AddDynamic(this, &ADryBones::OnHit);

    FrontOverlap = CreateDefaultSubobject<UBoxComponent>("DryBonesFrontOverlap");
    FrontOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    FrontOverlap->SetCollisionProfileName("OverlapAll");
    FrontOverlap->SetGenerateOverlapEvents(true);
    FrontOverlap->OnComponentBeginOverlap.AddDynamic(this, &ADryBones::OnForwardOverlap);
    FrontOverlap->SetupAttachment(RootComponent);

    BackOverlap = CreateDefaultSubobject<UBoxComponent>("DryBonesOverlap");
    BackOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BackOverlap->SetCollisionProfileName("OverlapAll");
    BackOverlap->SetGenerateOverlapEvents(true);
    BackOverlap->OnComponentBeginOverlap.AddDynamic(this, &ADryBones::OnBackwardOverlap);
    BackOverlap->SetupAttachment(RootComponent);

    Type = EEnemyType::DryBones;
    Tags.Add("DryBones");
}

void ADryBones::BeginPlay()
{
    Super::BeginPlay();

    SetState(EDryBonesState::Walking);
}

void ADryBones::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (State == EDryBonesState::Walking)
    {
        FVector location = GetActorLocation();
        location += Velocity * DeltaTime;
        SetActorLocation(location);
    }
    else if (State == EDryBonesState::Fallen)
    {
        FallenDuration -= DeltaTime;
        if (FallenDuration < 0.0f)
        {
            FallenDuration = 0.0f;
            SetState(EDryBonesState::Stunned);
        }
    }
    else if (State == EDryBonesState::Stunned)
    {
        StunnedDuration -= DeltaTime;
        if (StunnedDuration < 0.0f)
        {
            StunnedDuration = 0.0f;
            SetState(EDryBonesState::Stirring);
        }
    }
    else if (State == EDryBonesState::Stirring)
    {
        StirringDuration -= DeltaTime;

        if (StirringDuration < 0.0f)
        {
            StirringDuration = 0.0f;
            SetState(EDryBonesState::Walking);
        }
    }
    else if (State == EDryBonesState::Shotted)
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

void ADryBones::SetState(EDryBonesState state)
{
    if (State != state)
    {
        State = state;

        if (State == EDryBonesState::Walking)
        {
            BoxComponent->SetSimulatePhysics(true);
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            BoxComponent->SetCollisionProfileName("EnemyDynamic");
            FrontOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            BackOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            AddFlipBookOffset();
            ApplyInitialVelocity();
        }
        else if (State == EDryBonesState::Fallen)
        {
            SubtractFlipbookOffset();
            BoxComponent->SetSimulatePhysics(false);
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            BoxComponent->SetCollisionProfileName("OverlapAll");
            FrontOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            BackOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            FallenDuration = EnemyConstants::DryBonesFallenDuration;
        }
        else if (State == EDryBonesState::Stunned)
        {
            StunnedDuration = EnemyConstants::DryBonesStunnedDuration;
        }
        else if (State == EDryBonesState::Stirring)
        {
            StirringDuration = EnemyConstants::DryBonesStirringDuration;
        }
        else if (State == EDryBonesState::Shotted)
        {
            //Velocity.Z = -EnemyConstants::BulletBillSpeed;
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            FlipbookComponent->SetCollisionProfileName("NoCollision");
            BoxComponent->SetSimulatePhysics(true);
        }
        else if (State == EDryBonesState::Exploded)
        {
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            FlipbookComponent->SetCollisionProfileName("NoCollision");
            BoxComponent->SetSimulatePhysics(true);
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

            EGibType types[kNumBits] = { EGibType::DryBonesHead,
                                                        EGibType::Bone,
                                                        EGibType::Bone,
                                                        EGibType::Bone };

            for (int32 i = 0; i < kNumBits; i++)
            {
                bits[i] = GetWorld()->SpawnActor<AGib>(GibTemplate, locations[i], FRotator(0.0f, 0.0f, 0.0f));
                bits[i]->Spawn(types[i], impulses[i]);
            }

            Destroy();
        }

        UpdateFlipbook();
    }
}

void ADryBones::SubtractFlipbookOffset()
{
    FVector flipbookoffset = FlipbookComponent->GetRelativeLocation();
    flipbookoffset.Z = -9.0f;
    FlipbookComponent->SetRelativeLocation(flipbookoffset);
}

void ADryBones::AddFlipBookOffset()
{
    FVector flipbookoffset = FlipbookComponent->GetRelativeLocation();
    flipbookoffset.Z = 9.0f;
    FlipbookComponent->SetRelativeLocation(flipbookoffset);
}

void ADryBones::OnForwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("Goomba") || OtherActor->ActorHasTag("MushroomPickup") || OtherActor->ActorHasTag("DryBones") || OtherActor->ActorHasTag("Pickup") || OtherActor->ActorHasTag("BoomBoom") || OtherActor->ActorHasTag("Bobomb"))
    {
        BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        Velocity.X = -EnemyConstants::DryBonesSpeed;//right
    }
    else if (OtherActor->ActorHasTag("Bullet"))
    {
        SetState(EDryBonesState::Shotted);
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        SetState(EDryBonesState::Exploded);
    }
    UpdateFlipbook();
}

void ADryBones::OnBackwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("Goomba") || OtherActor->ActorHasTag("MushroomPickup") || OtherActor->ActorHasTag("DryBones") || OtherActor->ActorHasTag("Pickup") || OtherActor->ActorHasTag("BoomBoom") || OtherActor->ActorHasTag("Bobomb"))
    {
        BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        Velocity.X = EnemyConstants::DryBonesSpeed;//left
    }
    else if (OtherActor->ActorHasTag("Bullet"))
    {
        SetState(EDryBonesState::Shotted);
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        SetState(EDryBonesState::Exploded);
    }
    UpdateFlipbook();
}

void ADryBones::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor->ActorHasTag("MarioCharacter"))
    {
        if ((Hit.Normal.X <= -0.8f || Hit.Normal.X >= 0.8f) && (State == EDryBonesState::Walking))
        {
            // if hit on side
            Cast<AMarioCharacter>(OtherActor)->HandleDamage();
        }
        else if (Hit.Normal.Z >= 0.7f && State == EDryBonesState::Walking)
        {
            // if landed on mario
            Cast<AMarioCharacter>(OtherActor)->HandleDamage();
        }
        else if (Hit.Normal.Z <= -0.7f)
        {
            // Mario landed on Goomba
            if (State == EDryBonesState::Walking)
            {
                SetState(EDryBonesState::Fallen);
                Cast<AMarioCharacter>(OtherActor)->ApplyBounce();
            }
        }
    }
    else if (OtherActor->ActorHasTag("Bullet"))
    {
        SetState(EDryBonesState::Shotted);
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        SetState(EDryBonesState::Exploded);
    }
}

void ADryBones::ApplyInitialVelocity()
{
    if (Velocity == FVector(NULL, NULL, NULL))//moves at same direction no matter
    {
        if (InitialDirection == EDryBonesInitialDirection::Random)
        {
            int32 index = FMath::RandRange(0, 9) % 2;
            float speeds[] = { EnemyConstants::DryBonesSpeed, -EnemyConstants::DryBonesSpeed };
            Velocity.X = speeds[index];
        }
        else if (InitialDirection == EDryBonesInitialDirection::Right)
        {
            Velocity.X = -EnemyConstants::DryBonesSpeed;
        }
        else if (InitialDirection == EDryBonesInitialDirection::Left)
        {
            Velocity.X = EnemyConstants::DryBonesSpeed;
        }
    }
}

void ADryBones::UpdateFlipbook()
{
    UPaperFlipbook* flipbook = nullptr;

    if (State == EDryBonesState::Walking || State == EDryBonesState::Shotted)
    {
        flipbook = WalkingFlipbook;

        if (Velocity.X > 0.0f)
            FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
        else if (Velocity.X < 0.0f)
            FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
    }
    else if (State == EDryBonesState::Fallen || State == EDryBonesState::Shotted)
    {
        flipbook = FallenFlipbook;
    }
    else if (State == EDryBonesState::Stunned || State == EDryBonesState::Shotted)
    {
        flipbook = StunnedFlipbook;
    }
    else if (State == EDryBonesState::Stirring || State == EDryBonesState::Shotted)
    {
        flipbook = StirringFlipbook;
    }

    FlipbookComponent->SetFlipbook(flipbook);
    FlipbookComponent->PlayFromStart();
}
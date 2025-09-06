// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBill.h"
#include "EnemyConstants.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "MarioCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Poof.h"

ABulletBill::ABulletBill() :
    Velocity(),
    State(EBulletBillState::Unknown),
    CollisionCheck(nullptr),
    TopCheck(nullptr),
    AimmingTimer(0.0f),
    PeekingTimer(0.0f),
    Turn(0.0f),
    LifeTime(0.0f)
{
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionProfileName("OverlapAll");
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetNotifyRigidBodyCollision(true);
    BoxComponent->OnComponentHit.AddDynamic(this, &ABulletBill::OnHit);

    CollisionCheck = CreateDefaultSubobject<UBoxComponent>("BulletBillCheckOverlap");
    CollisionCheck->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CollisionCheck->SetCollisionProfileName("OverlapAll");
    CollisionCheck->SetGenerateOverlapEvents(true);
    CollisionCheck->OnComponentBeginOverlap.AddDynamic(this, &ABulletBill::OnCheckOverlap);
    CollisionCheck->SetupAttachment(RootComponent);

    TopCheck = CreateDefaultSubobject<UBoxComponent>("BulletBillTopOverlap");
    TopCheck->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TopCheck->SetCollisionProfileName("OverlapAll");
    TopCheck->SetGenerateOverlapEvents(true);
    TopCheck->OnComponentBeginOverlap.AddDynamic(this, &ABulletBill::OnTopOverlap);
    TopCheck->SetupAttachment(RootComponent);

    Tags.Add("BulletBill");
}

void ABulletBill::BeginPlay()
{
    Super::BeginPlay();

    if (ShootSound != nullptr)
        UGameplayStatics::PrimeSound(ShootSound);

    SetState(EBulletBillState::Peeking);
}

void ABulletBill::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (State == EBulletBillState::Peeking)
    {
        FVector location = GetActorLocation();
        location += Turn * Velocity * DeltaTime;
        SetActorLocation(location);
        PeekingTimer -= DeltaTime;
        if (PeekingTimer < 0.0f)
        {
            PeekingTimer = 0.0f;
            SetState(EBulletBillState::Aimming);
        }
    }
    else if (State == EBulletBillState::Aimming)
    {
        AimmingTimer -= DeltaTime;
        if (AimmingTimer < 0.0f)
        {
            AimmingTimer = 0.0f;
            SetState(EBulletBillState::Active);
        }
    }
    else if (State == EBulletBillState::Active)
    {
        FVector location = GetActorLocation();
        location += Turn * Velocity * DeltaTime;
        SetActorLocation(location);

        LifeTime -= DeltaTime;
        if (LifeTime < 0.0f)
        {
            Destroy();
        }
    }
    else if (State == EBulletBillState::Squished)
    {
        FVector location = GetActorLocation();
        location.Z += Velocity.Z * DeltaTime;
        SetActorLocation(location);
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

void ABulletBill::Spawn(float direction)
{
    FVector Start = GetActorLocation();
    Turn = direction;
    if (Turn > 0)
        FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
    else if (Turn < 0)
        FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    SetActorLocation(Start);
}

void ABulletBill::SetState(EBulletBillState state)
{
    if (State != state)
    {
        State = state;
        if (State == EBulletBillState::Peeking)
        {
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            FlipbookComponent->SetCollisionProfileName("NoCollision");
            Velocity.X = EnemyConstants::BulletBillSpeed;
            PeekingTimer = EnemyConstants::BulletBillPeekingTimer;
        }
        else if (State == EBulletBillState::Aimming)
        {
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            FlipbookComponent->SetCollisionProfileName("NoCollision");
            AimmingTimer = EnemyConstants::BulletBillAimmingTimer;
        }
        else if (State == EBulletBillState::Active)
        {
            if (ShootSound != nullptr)
                UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation());

            FVector location = GetActorLocation();
            APoof* pPoof = GetWorld()->SpawnActor<APoof>(PoofTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
            pPoof->Spawn();

            BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            FlipbookComponent->SetCollisionProfileName("EnemyDynamic");
            Velocity.X = EnemyConstants::BulletBillSpeed;
            LifeTime = EnemyConstants::BulletBillLifeTime;
        }
        else if (State == EBulletBillState::Squished)
        {
            //Velocity.Z = -EnemyConstants::BulletBillSpeed;
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            FlipbookComponent->SetCollisionProfileName("NoCollision");
            BoxComponent->SetSimulatePhysics(true);
        }
    }
}

void ABulletBill::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor->ActorHasTag("MarioCharacter") && (State == EBulletBillState::Active))
    {
        if ((Hit.Normal.X <= -0.8f || Hit.Normal.X >= 0.8f))
        {
            // if hit on side
            Cast<AMarioCharacter>(OtherActor)->HandleDamage();
        }
        else if (Hit.Normal.Z >= 0.7f && State == EBulletBillState::Active)
        {
            // if landed on mario
            Cast<AMarioCharacter>(OtherActor)->HandleDamage();
        }
        else if (Hit.Normal.Z <= -0.7f)
        {
            // Mario landed on
            SetState(EBulletBillState::Squished);
            Cast<AMarioCharacter>(OtherActor)->ApplyBounce();
        }
    }
    else if (OtherActor->ActorHasTag("Fireball") && (State == EBulletBillState::Active))
    {
        SetState(EBulletBillState::Squished);
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        Destroy();
    }
}

void ABulletBill::OnCheckOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("MarioCharacter") && (State == EBulletBillState::Active))
    {
        Cast<AMarioCharacter>(OtherActor)->HandleDamage();
    }
    else if (OtherActor->ActorHasTag("Fireball") || OtherActor->ActorHasTag("Bullet") && (State == EBulletBillState::Active))
    {
        SetState(EBulletBillState::Squished);
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        Destroy();
    }
}

void ABulletBill::OnTopOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Fireball") || OtherActor->ActorHasTag("Bullet") && (State == EBulletBillState::Active))
    {
        SetState(EBulletBillState::Squished);
        OtherActor->Destroy();
    }
    else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
    {
        Destroy();
    }
    else if (OtherActor->ActorHasTag("MarioCharacter") && (State == EBulletBillState::Active))
    {
        SetState(EBulletBillState::Squished);
        Cast<AMarioCharacter>(OtherActor)->ApplyBounce();
    }
}

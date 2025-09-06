// Fill out your copyright notice in the Description page of Project Settings.


#include "Nipper.h"
#include "EnemyConstants.h"
#include "MarioCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

ANipper::ANipper() :
    StraightFlipbook(nullptr),
    UpFlipbook(nullptr),
	State(ENipperState::Unknown),
	Direction(ENipperDirection::Unknown)
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ANipper::OnHit);
	BoxComponent->BodyInstance.bLockXTranslation = true;

	Check = CreateDefaultSubobject<UBoxComponent>("CheckOverlap");
	Check->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Check->SetCollisionProfileName("OverlapAll");
	Check->SetGenerateOverlapEvents(true);
	Check->OnComponentBeginOverlap.AddDynamic(this, &ANipper::CheckHit);
	Check->SetupAttachment(RootComponent);
}

void ANipper::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetActorLocation();
	SetState(ENipperState::Active);
}

void ANipper::OnHit(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		Cast<AMarioCharacter>(OtherActor)->HandleDamage();
	}
	else if (OtherActor->ActorHasTag("Fireball") || OtherActor->ActorHasTag("Bullet"))
	{
		SetState(ENipperState::Squished);
	}
	else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
	{
		Destroy();
	}
}

void ANipper::CheckHit(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		Cast<AMarioCharacter>(OtherActor)->HandleDamage();
	}
}

void ANipper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == ENipperState::Active)
	{
		AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();
		FVector marioLoc = mario->GetActorLocation();
		if (marioLoc.X < GetActorLocation().X - 32.0f)
		{
			SetDirection(ENipperDirection::Left);
		}
		else if (marioLoc.X > GetActorLocation().X + 32.0f)
		{
			SetDirection(ENipperDirection::Right);
		}
		else if (marioLoc.X <= GetActorLocation().X + 32.0f &&
			marioLoc.X >= GetActorLocation().X - 32.0f)
		{
			SetDirection(ENipperDirection::Up);
			FVector loc = GetActorLocation();
			loc.Z += Velocity.Z * DeltaTime;
			loc.X = OriginalLocation.X;
			SetActorLocation(loc);
		}

		if (GetActorLocation().Z > OriginalLocation.Z + EnemyConstants::NipperApex)
		{
			Velocity.Z = 0.0f;
		}
		else if (GetActorLocation().Z < OriginalLocation.Z + 16.0f)
		{
			Velocity.Z = EnemyConstants::NipperSpeed;
		}
	}
	else if (State == ENipperState::Squished)
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

void ANipper::SetState(ENipperState state)
{
	if (State != state)
	{
		State = state;
		if (State == ENipperState::Active)
		{
			Velocity.Z = EnemyConstants::NipperSpeed;
		}
		else if (State == ENipperState::Squished)
		{
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			FlipbookComponent->SetCollisionProfileName("NoCollision");
			BoxComponent->SetSimulatePhysics(true);
		}
	}
}

void ANipper::SetDirection(ENipperDirection direction)
{
	if (Direction != direction)
	{
		Direction = direction;
		if (Direction == ENipperDirection::Left)
		{
			FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (Direction == ENipperDirection::Right)
		{
			FlipbookComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		}
		UpdateFlipbook();
	}
}

void ANipper::UpdateFlipbook()
{
	UPaperFlipbook* flipbook = nullptr;

	if (State == ENipperState::Active)
	{
		if (Direction == ENipperDirection::Left || Direction == ENipperDirection::Right)
		{
			flipbook = StraightFlipbook;
		}
		else if (Direction == ENipperDirection::Up)
		{
			flipbook = UpFlipbook;
		}
	}
	else if (State == ENipperState::Squished)
	{
		flipbook = StraightFlipbook;
	}
	FlipbookComponent->SetFlipbook(flipbook);
	FlipbookComponent->PlayFromStart();
}

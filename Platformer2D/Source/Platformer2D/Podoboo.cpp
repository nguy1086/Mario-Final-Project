// Fill out your copyright notice in the Description page of Project Settings.


#include "Podoboo.h"
#include "EnemyConstants.h"
#include "MarioCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Splash.h"

APodoboo::APodoboo() :
	WaitTimer(0.0f),
	State(EPodobooState::Unknown),
	Velocity(),
	Apex(0.0f),
	OriginalLocation(),
	CheckOverlap(nullptr),
	SplashDelay(0.0f)
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName("EnemyDynamic");
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->OnComponentHit.AddDynamic(this, &APodoboo::OnHit);
	BoxComponent->SetEnableGravity(false);
	BoxComponent->BodyInstance.bLockXTranslation = true;

	CheckOverlap = CreateDefaultSubobject<UBoxComponent>("PodobooOverlap");
	CheckOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CheckOverlap->SetCollisionProfileName("OverlapAll");
	CheckOverlap->SetGenerateOverlapEvents(true);
	CheckOverlap->OnComponentBeginOverlap.AddDynamic(this, &APodoboo::Overlap);
	CheckOverlap->SetupAttachment(RootComponent);

	Type = EEnemyType::Podoboo;
	Tags.Add("Podoboo");
}

void APodoboo::BeginPlay()
{
	Super::BeginPlay();
	OriginalLocation = GetActorLocation();

	int32 index = FMath::RandRange(0, 9) % 2;
	float apex[] = { EnemyConstants::PodobooApexMin, EnemyConstants::PodobooApexMin };
	float temp = apex[index];
	

	Apex = OriginalLocation.Z + temp;
	State = EPodobooState::Waiting;
}

void APodoboo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EPodobooState::Waiting)
	{
		WaitTimer -= DeltaTime;
		if (WaitTimer < 0.0f)
		{
			WaitTimer = 0.0f;
			SetState(EPodobooState::Jumping);
		}
	}
	else if (State == EPodobooState::Jumping)
	{
		FVector location = GetActorLocation();
		location += Velocity * DeltaTime;
		SetActorLocation(location);
		if (Velocity.Z >= EnemyConstants::PodobooMinSpeed)
		{
			Velocity.Z -= 1.0f * 225.0f * DeltaTime;
		}
		else
		{
			Velocity.Z = EnemyConstants::PodobooMinSpeed;
		}

		if (location.Z >= Apex)
		{
			FlipbookComponent->SetRelativeRotation(FRotator(180.0f, 0, 0));
			SetState(EPodobooState::Dropping);
		}
	}
	else if (State == EPodobooState::Dropping)
	{
		FVector location = GetActorLocation();
		location += Velocity * DeltaTime;
		SetActorLocation(location);
		if (Velocity.Z <= -EnemyConstants::PodobooSpeed)
		{
			Velocity.Z = -EnemyConstants::PodobooSpeed;
		}
		else
		{
			Velocity.Z -= 1.0f * 500.0f * DeltaTime;
		}

		if (location.Z <= OriginalLocation.Z)
		{
			FlipbookComponent->SetRelativeRotation(FRotator(0, 0, 0));
			SetActorLocation(OriginalLocation);
			SetState(EPodobooState::Waiting);
		}
	}
	else if (State == EPodobooState::Shotted)
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
	SplashDelay -= DeltaTime;
}

void APodoboo::SetState(EPodobooState state)
{
	if (State != state)
	{
		State = state;

		if (State == EPodobooState::Waiting)
		{
			WaitTimer = FMath::RandRange(EnemyConstants::PodobooJumpMin, EnemyConstants::PodobooJumpMax);
		}
		else if (State == EPodobooState::Jumping)
		{
			Velocity.Z = EnemyConstants::PodobooSpeed;
		}
		else if (State == EPodobooState::Dropping)
		{
			Velocity.Z = 0;
		}
		else if (State == EPodobooState::Shotted)
		{
			//Velocity.Z = -EnemyConstants::BulletBillSpeed;
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			FlipbookComponent->SetCollisionProfileName("NoCollision");
			BoxComponent->SetSimulatePhysics(true);
		}
	}
}

void APodoboo::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if (OtherActor->ActorHasTag("MarioCharacter"))
	//{
	//	Cast<AMarioCharacter>(OtherActor)->HandleDamage();
	//}
	//else if (OtherActor->ActorHasTag("Lava") && SplashDelay <= 0.0f)
	//{
	//	SplashDelay = 0.5f;
	//	FVector location = GetActorLocation();
	//	if (State == EPodobooState::Jumping)
	//	{
	//		ASplash* splash = GetWorld()->SpawnActor<ASplash>(SplashTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
	//		splash->SpawnTop();
	//	}
	//	else if (State == EPodobooState::Dropping)
	//	{
	//		ASplash* splash = GetWorld()->SpawnActor<ASplash>(SplashTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
	//		splash->SpawnBottom();
	//	}
	//}
}

void APodoboo::Overlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Cast<AMarioCharacter>(OtherActor)->HandleDamage();
	}
	else if (OtherActor->ActorHasTag("Lava") && SplashDelay <= 0.0f)
	{
		SplashDelay = 0.5f;
		FVector location = GetActorLocation();
		if (State == EPodobooState::Jumping)
		{
			ASplash* splash = GetWorld()->SpawnActor<ASplash>(SplashTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
			splash->SpawnTop();
		}
		else if (State == EPodobooState::Dropping)
		{
			ASplash* splash = GetWorld()->SpawnActor<ASplash>(SplashTemplate, location, FRotator(0.0f, 0.0f, 0.0f));
			splash->SpawnBottom();
		}
	}
	else if (OtherActor->ActorHasTag("Bullet"))
	{
		SetState(EPodobooState::Shotted);
		OtherActor->Destroy();
	}
	else if (OtherActor->ActorHasTag("Rocket") || OtherActor->ActorHasTag("Explosion"))
	{
		Destroy();
	}
	else if (OtherActor->ActorHasTag("Enemy"))
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}
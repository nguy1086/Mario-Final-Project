// Fill out your copyright notice in the Description page of Project Settings.


#include "Disc.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "MarioCharacter.h"
#include "EnemyConstants.h"
// Sets default values
ADisc::ADisc() :
	Velocity(),
	Radius(0.0f),
	Angle(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("DiscComponent");
	BoxComponent->SetCollisionProfileName("Disc");
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->OnComponentHit.AddDynamic(this, &ADisc::OnHit);
	RootComponent = BoxComponent;

	CheckOverlap = CreateDefaultSubobject<UBoxComponent>("ThwompOverlap");
	CheckOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CheckOverlap->SetCollisionProfileName("OverlapAll");
	CheckOverlap->SetGenerateOverlapEvents(true);
	CheckOverlap->OnComponentBeginOverlap.AddDynamic(this, &ADisc::Overlap);
	CheckOverlap->SetupAttachment(RootComponent);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("DiscFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	Tags.Add("Disc");
}

void ADisc::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		Cast<AMarioCharacter>(OtherActor)->HandleDamage();
	}
	else if (OtherActor->ActorHasTag("Bullet"))
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FlipbookComponent->SetCollisionProfileName("NoCollision");
		BoxComponent->SetSimulatePhysics(true);
		State = EDiscState::Shotted;
	}
}

void ADisc::Overlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		Cast<AMarioCharacter>(OtherActor)->HandleDamage();
	}
	else if (OtherActor->ActorHasTag("Bullet"))
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FlipbookComponent->SetCollisionProfileName("NoCollision");
		BoxComponent->SetSimulatePhysics(true);
		State = EDiscState::Shotted;
	}
}

// Called every frame
void ADisc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EDiscState::Active)
	{
		Angle += FMath::DegreesToRadians(EnemyConstants::DiscSpeed) * DeltaTime;
		float x = Center.X + Radius * FMath::Cos(Angle);
		float z = Center.Z + Radius * FMath::Sin(Angle);

		SetActorLocation(FVector(x, GetActorLocation().Y, z));
	}
	else if (State == EDiscState::Shotted)
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

void ADisc::Spawn(float radius, FVector center)
{
	FVector Start = GetActorLocation();
	Radius = radius;
	Center = center;
	SetActorLocation(Start);
	State = EDiscState::Active;
}


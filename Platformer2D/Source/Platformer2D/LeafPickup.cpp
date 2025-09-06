// Fill out your copyright notice in the Description page of Project Settings.


#include "LeafPickup.h"
#include "PickupConstants.h"
#include "Math/UnrealMathUtility.h" 
#include "Components/BoxComponent.h"
#include "GameConstants.h"

ALeafPickup::ALeafPickup() :
	Overlap(nullptr),
	Velocity(),
	Angle(PI/2.0f)
{
	Overlap = CreateDefaultSubobject<UBoxComponent>("PickupFrontOverlap");
	Overlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Overlap->SetCollisionProfileName("OverlapAll");
	Overlap->SetGenerateOverlapEvents(true);
	Overlap->OnComponentBeginOverlap.AddDynamic(this, &ALeafPickup::OnOverlap);
	Overlap->SetupAttachment(RootComponent);

	Type = EPickupType::Leaf;
	Tags.Add("LeafPickup");
}

void ALeafPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EPickupState::Active)
	{
		FVector location = GetActorLocation();
		Angle += ((0.1f + (2.0f * PI))/1.5) * DeltaTime;
		FMath::DegreesToRadians(Angle);
		location.X += ((sin(Angle) * 3));
		location += Velocity * DeltaTime;
		SetActorLocation(location);

		if (location.Z < 0.0f)
			Destroy();
	}
}

void ALeafPickup::Spawn()
{
	State = EPickupState::Spawning;
	SpawnTime = PickupConstants::PickupAnimationDuration;
	Start = GetActorLocation();
	Target = Start;
	Target.Z += (GameConstants::TileSize * 3);
}

void ALeafPickup::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("MushroomPickup") || OtherActor->ActorHasTag("Goomba") || OtherActor->ActorHasTag("OneUpPickup") || OtherActor->ActorHasTag("FlowerPickup") || OtherActor->ActorHasTag("Enemy"))
	//{
	//	Velocity.Z = -PickupConstants::LeafSpeed;
	//}
}

void ALeafPickup::OnPickupActive()
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName("PickupDynamic");
	BoxComponent->SetSimulatePhysics(false);
	Velocity.Z = -PickupConstants::LeafSpeed;
}

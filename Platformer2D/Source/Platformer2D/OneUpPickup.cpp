// Fill out your copyright notice in the Description page of Project Settings.


#include "OneUpPickup.h"
#include "PickupConstants.h"
#include "Math/UnrealMathUtility.h" 
#include "Components/BoxComponent.h"


AOneUpPickup::AOneUpPickup() :
	FrontOverlap(nullptr),
	BackOverlap(nullptr),
	Velocity()
{
	FrontOverlap = CreateDefaultSubobject<UBoxComponent>("MovingPickupFrontOverlap");
	FrontOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FrontOverlap->SetCollisionProfileName("OverlapAll");
	FrontOverlap->SetGenerateOverlapEvents(true);
	FrontOverlap->OnComponentBeginOverlap.AddDynamic(this, &AOneUpPickup::OnForwardOverlap);
	FrontOverlap->SetupAttachment(RootComponent);

	BackOverlap = CreateDefaultSubobject<UBoxComponent>("MovingPickupBackOverlap");
	BackOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BackOverlap->SetCollisionProfileName("OverlapAll");
	BackOverlap->SetGenerateOverlapEvents(true);
	BackOverlap->OnComponentBeginOverlap.AddDynamic(this, &AOneUpPickup::OnBackwardOverlap);
	BackOverlap->SetupAttachment(RootComponent);

	Type = EPickupType::OneUp;
	Tags.Add("OneUpPickup");
}

void AOneUpPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EPickupState::Active)
	{
		FVector location = GetActorLocation();
		location += Velocity * DeltaTime;
		SetActorLocation(location);
	}
}

void AOneUpPickup::OnForwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("MushroomPickup") || OtherActor->ActorHasTag("Goomba") || OtherActor->ActorHasTag("OneUpPickup") || OtherActor->ActorHasTag("FlowerPickup") || OtherActor->ActorHasTag("Enemy"))
	{
		Velocity.X = -PickupConstants::MushroomSpeed;
	}
}

void AOneUpPickup::OnBackwardOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("World") || OtherActor->ActorHasTag("MushroomPickup") || OtherActor->ActorHasTag("Goomba") || OtherActor->ActorHasTag("OneUpPickup") || OtherActor->ActorHasTag("FlowerPickup") || OtherActor->ActorHasTag("Enemy"))
	{
		Velocity.X = PickupConstants::MushroomSpeed;
	}
}

void AOneUpPickup::OnPickupActive()
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName("PickupDynamic");
	BoxComponent->SetSimulatePhysics(true);

	int32 index = FMath::RandRange(0, 9) % 2;
	float speeds[] = { PickupConstants::MushroomSpeed, -PickupConstants::MushroomSpeed };
	Velocity.X = speeds[index];
}

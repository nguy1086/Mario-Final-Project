// Fill out your copyright notice in the Description page of Project Settings.


#include "Muncher.h"
#include "Components/BoxComponent.h"
#include "MarioCharacter.h"

AMuncher::AMuncher()
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->SetNotifyRigidBodyCollision(false);
	BoxComponent->OnComponentHit.AddDynamic(this, &AMuncher::OnHit);

	Check = CreateDefaultSubobject<UBoxComponent>("CheckOverlap");
	Check->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Check->SetCollisionProfileName("OverlapAll");
	Check->SetGenerateOverlapEvents(true);
	Check->OnComponentBeginOverlap.AddDynamic(this, &AMuncher::CheckHit);
	Check->SetupAttachment(RootComponent);

	Type = EEnemyType::Muncher;
	Tags.Add("Muncher");
}

void AMuncher::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		Cast<AMarioCharacter>(OtherActor)->HandleDamage();
	}
}

void AMuncher::CheckHit(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		Cast<AMarioCharacter>(OtherActor)->HandleDamage();
	}
}

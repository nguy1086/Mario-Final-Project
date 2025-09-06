// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowerPickup.h"
#include "PickupConstants.h"
#include "Components/BoxComponent.h"

AFlowerPickup::AFlowerPickup()
{
	Type = EPickupType::Flower;
	Tags.Add("FlowerPickup");
}

void AFlowerPickup::OnPickupActive()
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName("PickupDynamic");
	BoxComponent->SetSimulatePhysics(false);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GunPickup.h"
#include "PickupConstants.h"
#include "Components/BoxComponent.h"

AGunPickup::AGunPickup()
{
    Type = EPickupType::Gun;
    Tags.Add("Gun");
}

void AGunPickup::OnPickupActive()
{
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionProfileName("PickupDynamic");
    BoxComponent->SetSimulatePhysics(false);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketLauncherPickup.h"
#include "PickupConstants.h"
#include "Components/BoxComponent.h"

ARocketLauncherPickup::ARocketLauncherPickup()
{
    Type = EPickupType::Rocket;
    Tags.Add("RocketLauncher");
}

void ARocketLauncherPickup::OnPickupActive()
{
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionProfileName("PickupDynamic");
    BoxComponent->SetSimulatePhysics(false);
}
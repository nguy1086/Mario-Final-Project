// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinPickup.h"
#include "PaperFlipbookComponent.h"

ACoinPickup::ACoinPickup() :
	StaticCoinFlipbook(nullptr)
{
	State = EPickupState::Active;
	Type = EPickupType::Coin;
	Tags.Add("CoinPickup");
}

void ACoinPickup::BeginPlay()
{
	Super::BeginPlay();

	FlipbookComponent->PlayFromStart();
}

void ACoinPickup::UseStaticCoin()
{
	if(StaticCoinFlipbook != nullptr)
		FlipbookComponent->SetFlipbook(StaticCoinFlipbook);
	State = EPickupState::Active;
}

void ACoinPickup::UseMuncherCoin()
{
	if (StaticCoinFlipbook != nullptr)
		FlipbookComponent->SetFlipbook(StaticCoinFlipbook);
	State = EPickupState::Active;
	Tags.Add("MuncherCoin");
}

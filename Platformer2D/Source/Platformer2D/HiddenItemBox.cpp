// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddenItemBox.h"
#include "Components/BoxComponent.h"
#include "ItemBox.h"
#include "MarioPlayerState.h"
#include "MarioCharacter.h"
#include "Kismet/GameplayStatics.h"

AHiddenItemBox::AHiddenItemBox() :
	CheckTop(nullptr),
	CheckBottom(nullptr),
	Revealed(false)
{
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetSimulatePhysics(false);

	CheckTop = CreateDefaultSubobject<UBoxComponent>("TopOverlap");
	CheckTop->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CheckTop->SetCollisionProfileName("OverlapAll");
	CheckTop->SetGenerateOverlapEvents(true);
	CheckTop->OnComponentBeginOverlap.AddDynamic(this, &AHiddenItemBox::GoThroughOverlap);
	CheckTop->OnComponentEndOverlap.AddDynamic(this, &AHiddenItemBox::GoThroughOverlapEnded);
	CheckTop->SetupAttachment(RootComponent);

	CheckBottom = CreateDefaultSubobject<UBoxComponent>("BottomOverlap");
	CheckBottom->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CheckBottom->SetCollisionProfileName("OverlapAll");
	CheckBottom->SetGenerateOverlapEvents(true);
	CheckBottom->OnComponentBeginOverlap.AddDynamic(this, &AHiddenItemBox::BottomOverlapBegin);
	CheckBottom->OnComponentEndOverlap.AddDynamic(this, &AHiddenItemBox::BottomOverlap);
	CheckBottom->SetupAttachment(RootComponent);
}

void AHiddenItemBox::BeginPlay()
{
	Super::BeginPlay();

	if (ItemSound != nullptr)
		UGameplayStatics::PrimeSound(ItemSound);

	if (CoinSound != nullptr)
		UGameplayStatics::PrimeSound(CoinSound);

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHiddenItemBox::GoThroughOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		if (Revealed == false)
		{
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			CheckTop->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CheckTop->SetCollisionProfileName("NoCollision");
		}
	}
}

void AHiddenItemBox::GoThroughOverlapEnded(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		if (Revealed == false)
		{
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else
		{
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			CheckTop->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CheckTop->SetCollisionProfileName("NoCollision");
		}
	}
}

void AHiddenItemBox::BottomOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();
		AMarioPlayerState* PlayerState = mario->GetPlayerState<AMarioPlayerState>();
		if (PlayerState->State == EMarioState::Jumping)
		{
			if (BoxComponent->IsCollisionEnabled() == false)
			{
				BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

				Revealed = true;
			}
		}
	}
}

void AHiddenItemBox::BottomOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("MarioCharacter"))
	{
		AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();
		AMarioPlayerState* PlayerState = mario->GetPlayerState<AMarioPlayerState>();
		if (PlayerState->State == EMarioState::Jumping)
		{
			if (BoxComponent->IsCollisionEnabled() == false)
			{
				BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

				Revealed = true;
			}
		}
	}
}

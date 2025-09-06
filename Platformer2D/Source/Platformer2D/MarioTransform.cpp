// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioTransform.h"
#include "MarioPlayerState.h"
#include "PlatformerGameModeBase.h"
#include "PaperFlipbookComponent.h"


// Sets default values
AMarioTransform::AMarioTransform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("PickupFlipbook");
	FlipbookComponent->SetLooping(false);
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &AMarioTransform::OnFinishedPlaying);
	RootComponent = FlipbookComponent;
}

void AMarioTransform::SetFlipbook(EMarioForm oldForm, EMarioForm newForm, EMarioDirection direction)
{
	// Update the capsule half height, based on the current form
	if (oldForm == EMarioForm::Small && newForm == EMarioForm::Super)
	{
		FlipbookComponent->SetFlipbook(TransformFlipbook);
	}
	else if (oldForm == EMarioForm::Super && newForm == EMarioForm::Small)
	{
		FlipbookComponent->SetFlipbook(DamageFlipbook);
	}
	else if (oldForm == EMarioForm::Small || oldForm == EMarioForm::Super && newForm == EMarioForm::Fire || newForm == EMarioForm::Gun || newForm == EMarioForm::Rocket || newForm == EMarioForm::Raccoon)
	{
		FlipbookComponent->SetFlipbook(FireTransformFlipbook);
	}
	else if ((oldForm == EMarioForm::Fire || oldForm == EMarioForm::Gun || oldForm == EMarioForm::Rocket || oldForm == EMarioForm::Raccoon) && newForm == EMarioForm::Super)
	{
		FlipbookComponent->SetFlipbook(FireTransformFlipbook);
	}

	if (direction == EMarioDirection::Left)
		FlipbookComponent->SetRelativeScale3D(FVector(-1.0f, 1.0f, 1.0f));

	FlipbookComponent->PlayFromStart();
}

void AMarioTransform::OnFinishedPlaying()
{
	APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
	if (gameMode != nullptr)
		gameMode->OnMarioTransformEnded();
}


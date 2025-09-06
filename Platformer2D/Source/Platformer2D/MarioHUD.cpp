// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioHUD.h"
#include "Blueprint/UserWidget.h" 

AMarioHUD::AMarioHUD() :
	MarioHUDWidgetTemplate(nullptr)
{
}

void AMarioHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MarioHUDWidgetTemplate != nullptr)
	{
		UUserWidget* userWidget = CreateWidget(GetWorld(), MarioHUDWidgetTemplate);
		check(userWidget != nullptr);

		userWidget->AddToViewport();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGMC.h"
#include "Blueprint/UserWidget.h"

void AMainMenuGMC::BeginPlay()
{
	Super::BeginPlay();

	if (UIWidgetClass)
	{
		// Create the widget
		if (UUserWidget* UIWidget = CreateWidget<UUserWidget>(GetWorld(), UIWidgetClass))
		{
			// Add the widget to the viewport
			UIWidget->AddToViewport();
		}
	}
}


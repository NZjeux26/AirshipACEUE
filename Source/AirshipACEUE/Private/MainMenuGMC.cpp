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

			// Get the player controller
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				// Set input mode for UI only
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(UIWidget->TakeWidget()); // Correctly set focus to the created widget
				PlayerController->SetInputMode(InputMode);

				// Enable mouse cursor
				PlayerController->bShowMouseCursor = true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create UI Widget."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UIWidgetClass is null. Ensure it is set in the GameMode."));
	}
}



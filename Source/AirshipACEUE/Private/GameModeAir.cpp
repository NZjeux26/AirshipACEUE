// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeAir.h"

#include "AirGameInstance.h"
#include "Airship.h"
#include "AirshipController.h"
#include "Kismet/GameplayStatics.h"

AGameModeAir::AGameModeAir()
{
	// Set the default pawn to your Airship class
	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
	PlayerControllerClass = AAirshipController::StaticClass();
}

void AGameModeAir::BeginPlay()
{
	Super::BeginPlay();

	// Get the GameInstance
	if (UAirGameInstance* GI = Cast<UAirGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (GI->SelectedAirship)
		{
			// Spawn the selected airship as the player pawn
			FVector SpawnLocation = FVector(0.0f, 0.0f, 10.0f); // Adjust spawn location as needed
			FRotator SpawnRotation = FRotator::ZeroRotator;

			APawn* PlayerPawn = GetWorld()->SpawnActor<APawn>(GI->SelectedAirship, SpawnLocation, SpawnRotation);

			if (PlayerPawn)
			{
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				if (PlayerController)
				{
					PlayerController->Possess(PlayerPawn);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn the player airship."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No airship selected in GameInstance."));
		}
	}
}

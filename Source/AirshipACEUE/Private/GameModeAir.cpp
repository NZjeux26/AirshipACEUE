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

			AAirship* PlayerAirship = GetWorld()->SpawnActor<AAirship>(GI->SelectedAirship, SpawnLocation, SpawnRotation);

			if (PlayerAirship)
			{
				// Apply masses from GameInstance
				PlayerAirship->SetFuelMass(GI->FuelMass);
				PlayerAirship->SetCargoMass(GI->CargoMass);
				PlayerAirship->SetBallastMass(GI->BallastMass);
				PlayerAirship->SetWeaponsMass(GI->WeaponMass);

				// Recalculate total mass
				PlayerAirship->UpdateTotalMass();

				// Possess the airship
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				if (PlayerController)
				{
					// Reset the input mode to gameplay
					FInputModeGameOnly InputMode;
					PlayerController->SetInputMode(InputMode);

					// Hide the mouse cursor
					//PlayerController->bShowMouseCursor = false;
					//possess the player airship
					PlayerController->Possess(PlayerAirship);
				}

				UE_LOG(LogTemp, Log, TEXT("Player airship spawned and possessed successfully."));
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


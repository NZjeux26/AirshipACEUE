#include "AirGameInstance.h"
#include "Airship.h"
#include "WeaponHardpoint.h"

void UAirGameInstance::InitializeHardpointsFromAirship()
{
	AirshipLoadout.Empty(); // Clear previous loadouts

	if (!SelectedAirship)
	{
		UE_LOG(LogTemp, Error, TEXT("InitializeHardpointsFromAirship: No selected airship!"));
		return;
	}

	// Spawn a temporary airship to extract hardpoints
	AAirship* TempAirship = GetWorld()->SpawnActor<AAirship>(SelectedAirship);
	if (!TempAirship)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn temporary airship!"));
		return;
	}

	// Loop through detected hardpoints
	for (UWeaponHardpoint* Hardpoint : TempAirship->WeaponHardpoints)
	{
		if (!Hardpoint)
		{
			UE_LOG(LogTemp, Warning, TEXT("Skipping null hardpoint!"));
			continue;
		}

		FHardpointLoadout NewLoadout;
		NewLoadout.HardpointName = Hardpoint->GetName(); // Store name
		NewLoadout.Hardpoint = Hardpoint; // Store reference

		AirshipLoadout.Add(NewLoadout);
		UE_LOG(LogTemp, Log, TEXT("Added Hardpoint to Loadout: %s"), *NewLoadout.HardpointName);
	}

	UE_LOG(LogTemp, Log, TEXT("Initialized %d hardpoints for selected airship."), AirshipLoadout.Num());

	// Destroy the temporary airship (we only needed it to read data)
	TempAirship->Destroy();
}


#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HardpointLoadout.h" 
#include "AirGameInstance.generated.h"


UCLASS()
class AIRSHIPACEUE_API UAirGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	// Stores the selected airship blueprint class
	UPROPERTY(BlueprintReadWrite, Category = "Airship")
	TSubclassOf<class AAirship> SelectedAirship;
	// vars for storing the values selected by the user in the UI
	UPROPERTY(BlueprintReadWrite, Category = "Airship Properties")
	float FuelMass;

	UPROPERTY(BlueprintReadWrite, Category = "Airship Properties")
	float CargoMass;

	UPROPERTY(BlueprintReadWrite, Category = "Airship Properties")
	float BallastMass;

	UPROPERTY(BlueprintReadWrite, Category = "Airship Properties")
	float WeaponMass;

	UPROPERTY(BlueprintReadWrite, Category = "Loadout")
	TArray<FHardpointLoadout> AirshipLoadout; // Array of loadouts, one for each hardpoint

	// Function to initialize hardpoint data from selected airship
	void InitializeHardpointsFromAirship();
	
};

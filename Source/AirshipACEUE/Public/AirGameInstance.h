
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WeaponLoadout.h" 
#include "AirGameInstance.generated.h"


UCLASS()
class AIRSHIPACEUE_API UAirGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	// Stores the selected airship blueprint class
	UPROPERTY(BlueprintReadWrite, Category = "Airship")
	TSubclassOf<class APawn> SelectedAirship;
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
	TArray<FWeaponLoadout> WeaponLoadout; // Array of loadouts, one for each hardpoint
	
};

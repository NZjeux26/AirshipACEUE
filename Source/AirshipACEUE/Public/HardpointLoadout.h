#pragma once

#include "CoreMinimal.h"
#include "HardpointLoadout.generated.h"

// Struct to store information about each weapon loadout
USTRUCT(BlueprintType)
struct FHardpointLoadout
{
	GENERATED_BODY()

	// Hardpoint Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString HardpointName;
	
	UPROPERTY(BlueprintReadWrite, Category = "Loadout")
	class UWeaponHardpoint* Hardpoint;

	//selected weapon
	UPROPERTY(BlueprintReadWrite, Category = "Loadout")
	TSubclassOf<class AWeapon> SelectedWeapon;
	//selected Projectile
	UPROPERTY(BlueprintReadWrite, Category = "Loadout")
	TSubclassOf<class AProjectile> SelectedProjectile;

	UPROPERTY(BlueprintReadWrite, Category = "Loadout")
	int32 AmmoCount;

	// Default Constructor
	FHardpointLoadout()
		: SelectedWeapon(nullptr), SelectedProjectile(nullptr), AmmoCount(100) {}
};

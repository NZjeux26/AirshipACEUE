#pragma once

#include "CoreMinimal.h"
#include "WeaponLoadout.generated.h"

// Struct to store information about each weapon loadout
USTRUCT(BlueprintType)
struct FWeaponLoadout
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Loadout")
	class UWeaponHardpoint* Hardpoint;

	UPROPERTY(BlueprintReadWrite, Category = "Loadout")
	TSubclassOf<class AWeapon> Weapon;

	UPROPERTY(BlueprintReadWrite, Category = "Loadout")
	TSubclassOf<class AProjectile> Projectile;

	UPROPERTY(BlueprintReadWrite, Category = "Loadout")
	int32 Ammo;
};

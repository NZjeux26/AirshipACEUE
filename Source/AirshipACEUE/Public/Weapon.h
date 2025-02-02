// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AAirship;

UCLASS()
class AIRSHIPACEUE_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Dry mass of the weapon (kg)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	float DryMass;

	// Barrel length (meters)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	float BarrelLength;

	// Weapon type (optional for later categorization)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	int WeaponType;

	// Max ammo the weapon can carry
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmo;
	
	//The amount of ammuntion the weapon has onboard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 Ammo;

	// Ammo in the current magazine
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentMagazineAmmo;

	// Magazine size
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MagazineSize;

	// Reload time (seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	float ReloadTime;

	// Rate of fire (rounds per second)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	float RateOfFire;

	// Cartridge mass (kg)** Look at removing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float CartridgeMass;

	// Total ammo mass (calculated)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
	float TotalAmmoMass;

	//Muzzle velocity *Was going to use the projectile to store more muzzle velocity information but simpler to just put it in ehre instead
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	float MuzzleVelocity;

	// Number of crew required to operate the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	int32 CrewRequirement;

	// Projectile Blueprint class to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	TSubclassOf<AProjectile> ProjectileClass;

	FTimerHandle FireTimerHandle; // Timer handle to manage firing rate

	bool bCanFire = true; // Determines if weapon is allowed to fire

	void ResetFire(); // Function to reset fire availability
	FVector GetMuzzleLocation() const;
	FVector CalculateFireDirection(const FVector& MuzzleLocation, const FVector& CrosshairWorldPosition, const AAirship* Airship) const;
	AProjectile* SpawnProjectile(const FVector& MuzzleLocation, const FRotator& SpawnRotation);
	void BeginFireCooldown();
	void ConsumeAmmo();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Need a mesh for the weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;

	
	// Functions for firing and reloading
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();
};

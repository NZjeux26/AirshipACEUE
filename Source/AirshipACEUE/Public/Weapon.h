// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Need a mesh for the weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;
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

	// Base Muzzle velocity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	float MuzzleVelocity;

	// Number of crew required to operate the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	int32 CrewRequirement;

	// Projectile Blueprint class to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	TSubclassOf<AProjectile> ProjectileClass;

	// Functions for firing and reloading
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();
};

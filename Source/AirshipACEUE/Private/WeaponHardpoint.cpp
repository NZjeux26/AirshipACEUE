// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponHardpoint.h"

#include "Airship.h"
#include "Weapon.h"


// Sets default values for this component's properties
UWeaponHardpoint::UWeaponHardpoint(): MountedWeapon(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UWeaponHardpoint::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UWeaponHardpoint::AttachWeapon()
{
	MountedWeapon = nullptr;
	
	if (WeaponToMount && GetWorld())
	{
		// Spawn the weapon actor
		AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponToMount);
		if (SpawnedWeapon)
		{
			// Attach the weapon to this hardpoint
			SpawnedWeapon->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
            
			// Update weapon's location and rotation to match the hardpoint
			SpawnedWeapon->SetActorLocationAndRotation(GetComponentLocation(), GetComponentRotation());

			// 🔥 Set the weapon's owner to the airship that owns this hardpoint
			AAirship* OwningAirship = Cast<AAirship>(GetOwner());
			if (OwningAirship)
			{
				SpawnedWeapon->SetOwner(OwningAirship);
				UE_LOG(LogTemp, Warning, TEXT("Set owner of %s to %s"), *SpawnedWeapon->GetName(), *OwningAirship->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to set owner for weapon %s. Hardpoint's owner is NULL."), *SpawnedWeapon->GetName());
			}

			MountedWeapon = SpawnedWeapon;

			UE_LOG(LogTemp, Warning, TEXT("Weapon %s mounted on hardpoint %s"), *SpawnedWeapon->GetName(), *GetName());
			UE_LOG(LogTemp, Warning, TEXT("Hardpoint: %s Location: %s Rotation: %s"),
				   *GetName(),
				   *GetComponentLocation().ToString(),
				   *GetComponentRotation().ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn weapon on hardpoint %s"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponToMount not set for hardpoint %s"), *GetName());
	}
}

// Called every frame
void UWeaponHardpoint::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}


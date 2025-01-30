// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::Fire()
{
    UE_LOG(LogTemp, Log, TEXT("%s is attempting to fire"), *GetName());

    // Ensure ammo is available
    if (CurrentMagazineAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon %s is out of ammo!"), *GetName());
        return;
    }

    // Ensure the projectile class is set
    if (!ProjectileClass)
    {
        UE_LOG(LogTemp, Error, TEXT("Weapon %s has no ProjectileClass assigned!"), *GetName());
        return;
    }

    // Ensure WeaponMesh exists
    if (!WeaponMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Weapon %s has no WeaponMesh!"), *GetName());
        return;
    }

    // Get the muzzle location
    FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("Muzzle"));
    UE_LOG(LogTemp, Log, TEXT("Muzzle location: %s"), *MuzzleLocation.ToString());

    // Get the player controller
    APlayerController* PlayerController = nullptr;

    // Ensure the weapon has an owner (the airship)
    if (GetOwner())
    {
        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (OwnerPawn)
        {
            PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
        }
    }

    // If still null, try global method as a backup
    if (!PlayerController)
    {
        PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    }

    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("Weapon %s could not get player controller!"), *GetName());
        return;
    }
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("Weapon %s could not get player controller!"), *GetName());
        return;
    }

    float MouseX, MouseY;
    if (!PlayerController->GetMousePosition(MouseX, MouseY))
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon %s could not get mouse position!"), *GetName());
        return;
    }

    FVector WorldLocation, WorldDirection;
    if (!PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection))
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon %s could not deproject screen position!"), *GetName());
        return;
    }

    FVector AirshipWorldLocation = GetOwner()->GetActorLocation();
    FVector AimTarget = WorldLocation + (WorldDirection * 10000.0f);
    AimTarget.Y = AirshipWorldLocation.Y; // Lock to 2D plane
    FVector FireDirection = (AimTarget - MuzzleLocation).GetSafeNormal();

    UE_LOG(LogTemp, Log, TEXT("Fire direction: %s"), *FireDirection.ToString());

    // Spawn projectile
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, FireDirection.Rotation(), SpawnParams);
    if (!SpawnedProjectile)
    {
        UE_LOG(LogTemp, Error, TEXT("Weapon %s failed to spawn projectile!"), *GetName());
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Weapon %s successfully spawned a projectile!"), *GetName());

    // Initialize projectile
    SpawnedProjectile->InitialiseProjectile(MuzzleVelocity, FireDirection.Rotation().Pitch);
    
    // Reduce ammo count
    CurrentMagazineAmmo--;
}


void AWeapon::Reload()
{
	CurrentMagazineAmmo = MagazineSize;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Airship.h"


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
    if (CurrentMagazineAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Out of ammo!"));
        return;
    }

    if (!ProjectileClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ProjectileClass is NULL!"));
        return;
    }

    // Get the airship (assumes the weapon is attached to it)
    AAirship* Airship = Cast<AAirship>(GetOwner());
    if (!Airship)
    {
        UE_LOG(LogTemp, Error, TEXT("Fire(): Weapon has no valid Airship owner!"));
        return;
    }

    // Get the crosshair's global world position (assuming AAirship has a stored FVector for it)
    FVector CrosshairWorldPosition = Airship->GetCrosshairWorldPosition(); // Ensure this function exists in AAirship

    // Get the weapon's muzzle location
    if (!WeaponMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Fire(): WeaponMesh is NULL!"));
        return;
    }

    FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("Muzzle"));
    UE_LOG(LogTemp, Log, TEXT("Muzzle location: X=%f, Y=%f, Z=%f"), MuzzleLocation.X, MuzzleLocation.Y, MuzzleLocation.Z);
    UE_LOG(LogTemp, Log, TEXT("Crosshair world position: X=%f, Y=%f, Z=%f"), CrosshairWorldPosition.X, CrosshairWorldPosition.Y, CrosshairWorldPosition.Z);

    // Ensure projectile moves in the 2D plane by locking Y to match the airship's Y position
    FVector FireTarget = CrosshairWorldPosition;
    FireTarget.Y = Airship->GetActorLocation().Y; // Keep projectiles in 2D space

    // Calculate firing direction
    FVector FireDirection = (FireTarget - MuzzleLocation).GetSafeNormal();
    
    UE_LOG(LogTemp, Log, TEXT("Fire direction: X=%f, Y=%f, Z=%f"), FireDirection.X, FireDirection.Y, FireDirection.Z);

    // Spawn the projectile
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, FireDirection.Rotation(), SpawnParams);
    if (!SpawnedProjectile)
    {
        UE_LOG(LogTemp, Error, TEXT("Fire(): Failed to spawn projectile!"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Projectile spawned successfully: %s"), *SpawnedProjectile->GetName());
    UE_LOG(LogTemp, Log, TEXT("Projectile %s spawned at location: %s"), *SpawnedProjectile->GetName(), *SpawnedProjectile->GetActorLocation().ToString());

    //add a function to get the correct muzzle velocity here: *********
    // Set projectile velocity in the correct direction
    SpawnedProjectile->InitialiseProjectile(MuzzleVelocity, FireDirection.Rotation().Pitch);

    // Reduce ammo count
    //CurrentMagazineAmmo--;
}


void AWeapon::Reload()
{
	CurrentMagazineAmmo = MagazineSize;
}


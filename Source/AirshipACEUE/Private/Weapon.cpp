// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Airship.h"
#include "HLSLTree/HLSLTreeTypes.h"


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

void AWeapon::ResetFire()
{
    // Only allow firing if there's ammo in the magazine
    if (CurrentMagazineAmmo > 0)
    {
        bCanFire = true;
    }
    //UE_LOG(LogTemp, Log, TEXT("Weapon is ready to fire again"));
}

FVector AWeapon::GetMuzzleLocation() const
{
    // Get the location of the muzzle socket on the weapon mesh
    return WeaponMesh ? WeaponMesh->GetSocketLocation(TEXT("Muzzle")) : FVector::ZeroVector;
}

FVector AWeapon::CalculateFireDirection(const FVector& MuzzleLocation, const FVector& CrosshairWorldPosition,
    const AAirship* Airship) const
{
    // Adjust the target to ensure the projectile stays in the 2D plane by locking the Y-axis
    FVector FireTarget = CrosshairWorldPosition;
    // Lock the Y coordinate to the airship’s position to remain in 2D space.
    FireTarget.Y = Airship->GetActorLocation().Y;
    return (FireTarget - MuzzleLocation).GetSafeNormal();
}

AProjectile* AWeapon::SpawnProjectile(const FVector& MuzzleLocation, const FRotator& SpawnRotation)
{
    // Define spawn parameters for the projectile, ensuring it spawns even if collisions exist
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    return GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, SpawnRotation, SpawnParams);
}

void AWeapon::BeginFireCooldown()
{
    // Calculate fire cooldown based on rate of fire (converts RPS to seconds per shot)
    bCanFire = false;
    float FireCooldown = 1.0f / RateOfFire;
    GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AWeapon::ResetFire, FireCooldown, false);
   // UE_LOG(LogTemp, Log, TEXT("Weapon fired. Next shot in %f seconds"), FireCooldown);
}

void AWeapon::ConsumeAmmo()
{
    //consumes a round from the nagazine, if the magzine is zero, stop firing
    if (CurrentMagazineAmmo > 0)
    {
        CurrentMagazineAmmo--;
        // If ammo has reached 0 after decrementing, disable firing.
        if (CurrentMagazineAmmo == 0)
        {
            bCanFire = false;
        }
    }
    
    else bCanFire = false;
   // UE_LOG(LogTemp, Log, TEXT("MagSize: %d"), CurrentMagazineAmmo);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AWeapon::GetWeaponsMass() const
{
    return DryMass;
}

float AWeapon::GetMunitionsMass() const
{
    return CartridgeMass * Ammo;
}

void AWeapon::Fire()
{
    if (!bCanFire) // performs all the validation checks
    {
        return;
    }
    
    AAirship* Airship = Cast<AAirship>(GetOwner()); // Already validated in CanFire()

    FVector MuzzleLocation = GetMuzzleLocation();
    FVector CrosshairWorldPosition = Airship->GetCrosshairWorldPosition();
    FVector FireDirection = CalculateFireDirection(MuzzleLocation, CrosshairWorldPosition, Airship);

    // Spawn the projectile at the muzzle location with the calculated fire direction
    AProjectile* SpawnedProjectile = SpawnProjectile(MuzzleLocation, FireDirection.Rotation());
    if (!SpawnedProjectile)
    {
        UE_LOG(LogTemp, Error, TEXT("Fire(): Failed to spawn projectile!"));
        return;
    }

    // Initialise the projectile
    SpawnedProjectile->InitialiseProjectile(MuzzleVelocity, FireDirection.Rotation().Pitch);

    // Reduce ammo count
    ConsumeAmmo();

    // Start cooldown timer
    BeginFireCooldown();
}



void AWeapon::Reload()
{
    //this will need changed, I need a value for how much ammo is actually being carried and to subtract from that to fill the mag
    if (Ammo > MagazineSize )
    {
        CurrentMagazineAmmo = MagazineSize;
        Ammo - MagazineSize;
       // UE_LOG(LogTemp, Error, TEXT("Reloading! Mag Size now: %d"), CurrentMagazineAmmo);
        bCanFire = true;
    }
    else
    {
        bCanFire = false;
        //UE_LOG(LogTemp, Warning, TEXT("Out of Ammo for this Weapon!"));
    }
}

void AWeapon::SetProjectileClass(TSubclassOf<AProjectile> NewProjectileClass)
{
    ProjectileClass = NewProjectileClass;
}

void AWeapon::SetAmmo(int32 AmmoCount)
{
    Ammo = AmmoCount;
}


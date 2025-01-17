// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoCannon.h"


// Sets default values
AAutoCannon::AAutoCannon()
{
	/*autocannon = Weapons(
	dry_mass = 4.8,
	barrel_length = 3.25,
	max_ammo = 300,
	type = 1, 
	mag_size = 26,
	rate_of_fire = 4, # rounds per second
	catridge_mass = 0.221,
	reload_time = 2.5,
	muzzle_velocity = 100,
	crew_requirement = 4,
	proj = Projectile(0,0,0,0,0.096)
)*/
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DryMass = 4.8f;
	BarrelLength = 3.25f;
	MaxAmmo = 300;
	WeaponType = 1;
	MagazineSize = 26;
	RateOfFire = 4.0f;
	CartridgeMass = 0.221f; //total cartridge mass, projectile is slightly less
	ReloadTime = 2.5f;
	MuzzleVelocity = 100.0f;
	CrewRequirement = 4;
}

// Called when the game starts or when spawned
void AAutoCannon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAutoCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoCannon.h"


// Sets default values
AAutoCannon::AAutoCannon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DryMass = 1.0f;
	BarrelLength = 1.0f;
	MaxAmmo = 1;
	WeaponType = 1;
	MagazineSize = 1;
	RateOfFire = 1.0f;
	CartridgeMass = 1.0f; //total cartridge mass, projectile is slightly less
	ReloadTime = 1.0f;
	MuzzleVelocity = 1.0f;
	CrewRequirement = 1;

	// Create and set a SceneComponent as the root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaonMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetSimulatePhysics(false);
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


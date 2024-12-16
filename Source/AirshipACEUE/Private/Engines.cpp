// Fill out your copyright notice in the Description page of Project Settings.

#include "Engines.h"

// Sets default values for this component's properties
UEngines::UEngines()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Initialise default values
	Mass = 0.0f;         // Default engine mass in kg
	FuelFlow = 0.0f;       // Default fuel flow in kg/s
	PropDiameter = 0.0f;   // Default propeller diameter in meters
	PropEfficiency = 0.0f; // Default propeller efficiency (80%)
	PropArea = 0.0f;
	HorsePower = 0.0f;   // Default engine horsepower
	Thrust = FVector(0.0f,0.0f,0.0f);
}

void UEngines::CalculateEngineThrust(float AirDensity, const FVector& AirVelocity)
{
	// Calculate the thrust for each axis we care about (X and Z)
	constexpr float ExitVelocity = 3.5f; // Standstill exit velocity

	float ThrustX = 0.5f * AirDensity * PropArea * 
					(FMath::Pow(ExitVelocity, 2) - FMath::Pow(AirVelocity.X, 2));

	float ThrustZ = 0.5f * AirDensity * PropArea * 
					(FMath::Pow(ExitVelocity, 2) - FMath::Pow(AirVelocity.Z, 2));

	// Combine the thrusts into a single vector
	Thrust = FVector(ThrustX, 0.0f, ThrustZ);
}

// Called when the game starts
void UEngines::BeginPlay()
{
	Super::BeginPlay();
	PropArea = GetPropArea();
}

float UEngines::GetPropArea() const
{
	const float Radius = PropDiameter / 2.0f;
	return PI * FMath::Pow(Radius, 2);
}

// Called every frame
void UEngines::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


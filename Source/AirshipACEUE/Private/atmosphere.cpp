// Fill out your copyright notice in the Description page of Project Settings.
#include "atmosphere.h"
#include "physicsConstants.h"
#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/WinAdapter.h>

UAtmosphere::UAtmosphere()
{
	//sets the atmosphere to sea level standards as laid out in the ISA.
	Pressure = PhysicsConstants::GStandardPressureSeaLevelHecta; // Default pressure
	Density = PhysicsConstants::GAirDensitySeaLevel; // Default density
	Temperature = PhysicsConstants::GStandardTemperatureAtSeaLevelCelsius; // Default temperature
}

//I don't like how these were made const by GPT, I don't think they should be considering how much they change,*
float UAtmosphere::CalculateTemperature(float Altitude) const
{
	return PhysicsConstants::GStandardTemperatureAtSeaLevelCelsius -
		   (PhysicsConstants::GTemperatureLapseRate * Altitude);
}

float UAtmosphere::CalculatePressure(float Altitude) const
{
	float TemperatureKelvin = Temperature + 273.15f; //wants to be a const, pro vs con?
	//UE_LOG(LogTemp, Warning, TEXT("Pressure Temp is: %0.2f"),Temperature);
	return PhysicsConstants::GStandardPressureSeaLevelHecta *
		   FMath::Exp(-PhysicsConstants::GGravityOnEarth *
					  PhysicsConstants::GMolarMassOfAir *
					  Altitude /
					  (PhysicsConstants::GGasConstant * TemperatureKelvin));
}

float UAtmosphere::CalculateDensity() const
{
	float TemperatureKelvin = Temperature + 273.15f; // Convert temperature to Kelvin
	return (Pressure * PhysicsConstants::GMolarMassOfAir) /
		   (PhysicsConstants::GGasConstant * TemperatureKelvin) * 1000;
}

void UAtmosphere::UpdateAtmosphere(float Altitude)
{
	Temperature = CalculateTemperature(Altitude);
	Pressure = CalculatePressure(Altitude);
	Density = CalculateDensity();
}

void UAtmosphere::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("UAtmosphere subsystem initialised"));
}

// Called when the subsystem is deinitialized
void UAtmosphere::Deinitialize()
{
	Super::Deinitialize();

	// Add cleanup logic if needed
	UE_LOG(LogTemp, Warning, TEXT("BuoyancySubsystem deinitialised"));
}
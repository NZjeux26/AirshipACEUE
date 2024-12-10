#pragma once

namespace PhysicsConstants
{
	// Gravitational acceleration on Earth (m/s²)
	constexpr float GGravityOnEarth = 9.80665f;
	//gets the gravity value from Unreal's physics engine, it comes in at cm/s so convert to meters

	// Air properties
	constexpr float GAirDensitySeaLevel = 1.225f; // kg/m³
	constexpr float GHydrogenDensity = 0.008375f; // kg/m³
	constexpr float GMolarMassOfAir = 0.0289644f; // kg/mol
	constexpr float GMolarMassOfHydrogen = 0.00201588f; // kg/mol

	// Atmospheric pressure
	//constexpr float GStandardPressureSeaLevel = 101325.0f; // Pascals
	constexpr float GStandardPressureSeaLevelHecta = 101.325f; // Hectopascals

	// Gas properties
	constexpr float GGasConstant = 8.3144598f; // J/(kg·K)

	// Atmospheric lapse rate and temperature
	constexpr float GTemperatureLapseRate = 0.0065f; // K/m
	constexpr float GStandardTemperatureAtSeaLevelCelsius = 15.0f; // Celsius
	constexpr float GStandardTemperatureAtSeaLevelKelvin = 288.15f; // Kelvin

	// Miscellaneous constants
	constexpr float GExponentConstant = 5.2561f;
	constexpr float GInertiaCoefficient = 0.09f;
}

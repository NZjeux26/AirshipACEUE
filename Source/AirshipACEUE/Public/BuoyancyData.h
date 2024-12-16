#pragma once
#include "Airship.h"

class AIRSHIPACEUE_API BuoyancyData
{
public:
	// Calculate buoyancy force: F = (density - hydrogen_density) * gravity * volume
	static float CalculateBuoyancyForce(float Density, float Volume);
	// Calculate gravity force: F = mass * gravity
	static float CalculateGravityForce(float Mass); // not actually needed since this can be taken care of with Unreal
	//Calculate the frontal area of the airship
	static float CalFrontalArea(float Diameter);
	//Calculate the lateral area of the airship
	static float CalLateralArea(float Length,float Diameter);
	//**What about area in the Z axis?**
	static float CalVolume(float Length, float Diameter);
};

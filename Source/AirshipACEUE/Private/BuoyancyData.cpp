#include "BuoyancyData.h"
#include "physicsConstants.h"
#include "Math/UnrealMathUtility.h"
#include <cmath>
#include "EditorCategoryUtils.h"

float BuoyancyData::CalculateBuoyancyForce(float Density, float Volume)
{
	return (Density - PhysicsConstants::GHydrogenDensity) *
		   PhysicsConstants::GGravityOnEarth *
		   Volume;
}

float BuoyancyData::CalculateGravityForce(float Mass)
{
	return Mass * PhysicsConstants::GGravityOnEarth;
}

float BuoyancyData::CalFrontalArea(float Diameter)
{
	return PI * Diameter; // In the Python code and in the C code I use radius * 2, but radius * 2 is just the diameter
}

float BuoyancyData::CalLateralArea(float Length, float Diameter)
{
	//2 * math.pi * self.radius * self.length
	return 2 * PI * (Diameter / 2) * Length;
}

float BuoyancyData::CalVolume(float Length, float Diameter)
{
	//self.volume = math.pi * (diameter / 2)**2 * length
	return PI * FMath::Square(Diameter /2) * Length;
}

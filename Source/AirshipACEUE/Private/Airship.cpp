#include "Airship.h"
#include "physicsConstants.h"
#include "atmosphere.h"
#include "BuoyancyData.h"
#include "Camera/CameraActor.h"
#include "Math/UnitConversion.h"

// Sets default values
AAirship::AAirship()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and attach a static mesh component
	AirshipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AirshipMesh"));
	RootComponent = AirshipMesh;

	// Initialise default properties
	Length = 1.0f; //length in meters
	Diameter = 1.0f; //diameter in meters
	CD = 0.1f; //Drag coefficient of the airship
	Mass = 1.0f; //mass of the airship in KG
	Velocity = FVector::ZeroVector;
	//Base values set to one so cal values are not zero.
	//**WIll be adding much more but getting the basics in now**
	//**May be worth having fixed default values for testing**
}

// Called when the game starts or when spawned
void AAirship::BeginPlay()
{
	Super::BeginPlay();
	// Calculate dimensions from the mesh
	UpdateDimensionsFromMesh();
	Volume = BuoyancyData::CalVolume(Length, Diameter); //Automatically calculate the volume based off the other values
	FrontalArea = BuoyancyData::CalFrontalArea(Diameter); //Frontal area of the airship in meters squared
	LateralArea = BuoyancyData::CalLateralArea(Length, Diameter); //Lateral area of the airship in meters squared
	
	//Moved here since these should be updated on play after the editor has set the values in the BP
	// Apply mass to the physics system
	 if (AirshipMesh)
	 {
		AirshipMesh->SetEnableGravity(false);
	 	bool bIsGravityEnabled = AirshipMesh->IsGravityEnabled();
	 	UE_LOG(LogTemp, Warning, TEXT("Airship gravity enabled: %s"), bIsGravityEnabled ? TEXT("True") : TEXT("False"));
	 }
}

// Called every frame does all the checking against the atmo and the object
void AAirship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//gets the current position of the airship, assigns the Z location
	FVector CurrentLocation = GetActorLocation();
	float Altitude = CurrentLocation.Z; //this is actually in CM, but we can just treat it as Meters in the math?

	//check for the Atmosphere subsystem, if found assign local vars to update each tick by calling the cal functions to
	//update based on airship position.
	if (UAtmosphere* AtmosphereSub = GetWorld()->GetSubsystem<UAtmosphere>())
	{
		//update the atmo each frame
		AtmosphereSub->UpdateAtmosphere(Altitude);
		//get the new values
		float Temperature = AtmosphereSub->GetTemperature();
		float Pressure = AtmosphereSub->GetPressure();
		float Density = AtmosphereSub->GetDensity(); 
		//calculate the Bforce and Gforce
		float BuoyantForce = BuoyancyData::CalculateBuoyancyForce(Density, Volume);  
		float GravityForce = Mass * PhysicsConstants::GGravityOnEarth; //wait I might not need this because of the UE Gravity
		//The netforce acting on the object, includes Bforce,Gforce,Drag,Engines and recoil
		float NetForce = BuoyantForce - GravityForce; 
		float Acceletration = NetForce / Mass;
		
		Velocity.Z += Acceletration * DeltaTime;
		FVector BuoyancyVector = FVector(0.0f, 0.0f, Velocity.Z);

		AddActorWorldOffset(BuoyancyVector * DeltaTime, true); 
		
		//Debugging
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				0.0f,
				FColor::Yellow,
				FString::Printf(TEXT("Temperature: %.4f\nPressure: %.4f\nDensity: %0.4f\nNetForce: %.4f\nBForce: %0.4f\nGForce: %0.4f\nAlt: %.4f\nAcc: %.4f"),
											Temperature, Pressure, Density, NetForce, BuoyantForce, GravityForce,Altitude, Acceletration));
		}
	}
}

void AAirship::UpdateDimensionsFromMesh()
{
	if (AirshipMesh && AirshipMesh->GetStaticMesh())
	{
		// Get the bounding box of the mesh
		FBoxSphereBounds Bounds = AirshipMesh->Bounds;

		// Scale the bounding box dimensions
		FVector ScaledExtent = Bounds.BoxExtent * 2.0f; // Full size (extent is half the size)

		Length = ScaledExtent.X; // Assume length corresponds to the X-axis
		Diameter = FMath::Max(ScaledExtent.Y, ScaledExtent.Z); // Diameter from the largest cross-sectional axis
		float Radius = Diameter / 2.0f;

		// Calculate derived properties
		Volume = BuoyancyData::CalVolume(Length, Diameter);// Volume of a cylinder
		FrontalArea = BuoyancyData::CalFrontalArea(Diameter); //Frontal area of the airship in meters squared
		LateralArea = BuoyancyData::CalLateralArea(Length, Diameter); //Lateral area of the airship in meters squared       // Lateral surface area

		// Debug output for verification
		UE_LOG(LogTemp, Warning, TEXT("Airship Dimensions: Length=%f, Diameter=%f, Volume=%f"), Length, Diameter, Volume);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AirshipMesh is not set!"));
	}
}

void AAirship::SetAirshipScale(float ScaleFactor)
{
	// Set uniform scale
	SetActorScale3D(FVector(ScaleFactor));

	// Recalculate dimensions
	UpdateDimensionsFromMesh();
}

void AAirship::SetAirshipScale3D(FVector ScaleFactor)
{
	// Set non-uniform scale
	SetActorScale3D(ScaleFactor);

	// Recalculate dimensions
	UpdateDimensionsFromMesh();
}
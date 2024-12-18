#include "Airship.h"

#include "AirshipController.h"
#include "physicsConstants.h"
#include "atmosphere.h"
#include "BuoyancyData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engines.h"

// Sets default values
AAirship::AAirship()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and attach a static mesh component
	AirshipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AirshipMesh"));
	RootComponent = AirshipMesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(AirshipMesh);
	
	AirshipCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	AirshipCamera->SetupAttachment(SpringArm);

	MaxThrottle = 1.0f;
	MinThrottle = -1.0f;
	Throttle = 0.0f;
	// Initialise default properties
	Length = 1.0f; //length in meters
	Diameter = 1.0f; //diameter in meters
	CD = 0.1f; //Drag coefficient of the airship
	Mass = 1.0f; //mass of the airship in KG
	NumEngines = 0;
	Velocity = FVector::ZeroVector;
	Position = FVector::ZeroVector;
	
}

// Called when the game starts or when spawned
void AAirship::BeginPlay()
{
	Super::BeginPlay();
	// Calculate dimensions from the mesh commented out for now until meshes for players setup
	//UpdateDimensionsFromMesh(); 
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
	//Set the airship as the default pawn
	//SetDefaultPawn();
	
	// Populate Engines array with all attached engine components
	GetComponents(Engines);
	for (UEngines* Engine : Engines)
	{
		if (Engine)
		{
			UE_LOG(LogTemp, Log, TEXT("Engine found: %s"), *Engine->GetName());
			Engine->RegisterComponent(); // Ensure its act
		}
		else UE_LOG(LogTemp, Warning, TEXT("Engine not found"));
	}
	
	// if (GetWorld()->GetFirstPlayerController())
	// {
	// 	EnableInput(AAirshipController);
	// }
}

// Called every frame does all the checking against the atmo and the object
void AAirship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//gets the current position of the airship, assigns the Z location
	const FVector CurrentLocation = GetActorLocation();
	const float Altitude = CurrentLocation.Z; //this is actually in CM, but we can just treat it as Meters in the math?
	//check for the Atmosphere subsystem, if found assign local vars to update each tick by calling the cal functions to update based on airship position.
	if (UAtmosphere* AtmosphereSub = GetWorld()->GetSubsystem<UAtmosphere>())
	{
		//update the atmo each frame
		AtmosphereSub->UpdateAtmosphere(Altitude);
		//get the new values
		float Temperature = AtmosphereSub->GetTemperature();
		float Pressure = AtmosphereSub->GetPressure();
		float Density = AtmosphereSub->GetDensity(); 
		//calculate the Bforce and Gforce (now using Vectors)
		FVector BuoyantForce = FVector(0.0f, 0.0f, BuoyancyData::CalculateBuoyancyForce(Density, Volume));
		FVector GravityForce = FVector(0.0f, 0.0f, Mass * PhysicsConstants::GGravityOnEarth);
		FVector DragForce = AAirship::CalDrag(Density); //drag here is actually correct not the python version
		FVector Thrust = FVector::ZeroVector; 
		//Something for engine, calculate the engine thrust each frame and update thrust. It should decrease as altitude increases.
		for (UEngines* Engine : Engines)
		{
			if (Engine)
			{
				Engine->CalculateEngineThrust(Density, Velocity);
				Thrust = Engine->Thrust;
			}
		}
		FVector TotalThrust = Thrust * NumEngines; //total thrust is the thrust from each engine * the num of engines
		FVector Power = TotalThrust * Throttle;
		//The netforce acting on the object, includes Bforce,Gforce,Drag,Engines and recoil
		FVector NetForce = BuoyantForce - GravityForce - DragForce + Power; //plus now since the minus is in the vector itself
		FVector Acceleration = NetForce / Mass;
		
		Velocity += Acceleration * DeltaTime; //Euler integration, move to Verlet later
		Position = Velocity; //this may be wrong and may need to be actually with DT as well. (might be sorted when moved to Verlet)

		AddActorWorldOffset(Position * DeltaTime, true); 
		
		//Debugging
		if (GEngine)
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
				   -1,
				   0.0f,
				   FColor::Yellow,
				   FString::Printf(TEXT("Atmosphere Data:\n  - Temperature: %.4f\n  - Pressure: %.4f\n  - Density: %.4f\n"
						"Engines:\n  - Throttle: %.4f\n  - Total Thrust: (%.4f, %.4f, %.4f)\n  - Power: (%.4f, %.4f, %.4f)\n"
						"Forces:\n  - Net Force: (%.4f, %.4f, %.4f)\n  - Buoyant Force: (%.4f, %.4f, %.4f)\n"
						"  - Gravity Force: (%.4f, %.4f, %.4f)\n  - Drag Force: (%.4f, %.4f, %.4f)\n"
						"Physics:\n  - Altitude: %.4f\n  - Acceleration: (%.4f, %.4f, %.4f)\n  - Velocity: (%.4f, %.4f, %.4f)\n"),
						   Temperature, Pressure, Density,
						   Throttle,
						   TotalThrust.X, TotalThrust.Y, TotalThrust.Z,
						   Power.X, Power.Y, Power.Z,
						   NetForce.X, NetForce.Y, NetForce.Z,
						   BuoyantForce.X, BuoyantForce.Y, BuoyantForce.Z,
						   GravityForce.X, GravityForce.Y, GravityForce.Z,
						   DragForce.X, DragForce.Y, DragForce.Z,
						   Altitude,
						   Acceleration.X, Acceleration.Y, Acceleration.Z,
						   Velocity.X, Velocity.Y, Velocity.Z));
			}
	}
}

void AAirship::ClampThrottle()
{
	Throttle = FMath::Clamp(Throttle, MinThrottle, MaxThrottle);
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

void AAirship::SetDefaultPawn()
{
	// Set the Default Pawn Class dynamically
	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		GameMode->DefaultPawnClass = StaticClass();
	}

	// Possess the Airship
	if (AAirshipController* AirshipController = Cast<AAirshipController>(GetWorld()->GetFirstPlayerController()))
	{
		AirshipController->Possess(this);
		UE_LOG(LogTemp, Log, TEXT("PlayerController is now possessing the Airship: %s"), *GetName());
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("No PlayerController found!"));
	}
}
//return (density / 2) * self.yval**2 * self.cd * self.lateral_area
FVector AAirship::CalDrag(float Density) const
{
	float DragZ = Density / 2 * FMath::Square(Velocity.Z) * CD * LateralArea;
	float DragY = Density / 2 * FMath::Square(Velocity.Y) * CD * LateralArea;
	float DragX = Density / 2 * FMath::Square(Velocity.X) * CD * FrontalArea;
	return FVector(DragX,DragY,DragZ);//Y isn't actually used but done anyway
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

void AAirship::AddThrottle(float Value)
{
	Throttle += Value;
	Throttle = FMath::Clamp(Throttle, MinThrottle, MaxThrottle);
}



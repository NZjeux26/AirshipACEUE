#include "Airship.h"

#include "AirshipController.h"
#include "physicsConstants.h"
#include "atmosphere.h"
#include "BuoyancyData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engines.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AAirship::AAirship()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set a SceneComponent as the root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create and attach a static mesh component to the root
	AirshipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AirshipMesh"));
	AirshipMesh->SetupAttachment(RootComponent);
	
	// Configure collision for AirshipMesh
	AirshipMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AirshipMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	AirshipMesh->SetCollisionResponseToAllChannels(ECR_Block);  // Block everything by default
	AirshipMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	AirshipMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	// Create a SpringArm and attach to the root
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	// Setup the camera and attach to the spring arm
	AirshipCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	AirshipCamera->SetupAttachment(SpringArm);

	MaxThrottle = 100.0f;
	MinThrottle = -100.0f;
	Throttle = FVector::ZeroVector;

	// Initialise default properties
	Length = 1.0f; // length in meters
	Diameter = 1.0f; // diameter in meters
	CD = 0.1f; // Drag coefficient of the airship
	DryMass = 1.0f;
	FuelMass = 1.0f;
	CargoMass = 1.0f;
	WeaponMass = 1.0f;
	BallastMass = 1.0f;
	EngineMass = 1.0f;
	NumEngines = 0;
	Velocity = FVector::ZeroVector;
	Position = FVector::ZeroVector;
	TotalMass = 1.0f;

	// Smoothed values to get the avg of the values for display
	SmoothedVelocity = FVector::ZeroVector;
	SmoothedAcceleration = FVector::ZeroVector;
	SmoothedNetForce = FVector::ZeroVector;
	SmoothedThrust = FVector::ZeroVector;
	SmoothedPower = FVector::ZeroVector;
	SmoothedBForce = FVector::ZeroVector;
	SmoothedDragForce = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AAirship::BeginPlay()
{
	Super::BeginPlay();
	PreviousPosition = GetActorLocation();
	// Calculate dimensions from the mesh commented out for now until meshes for players setup
	//UpdateDimensionsFromMesh(); 
	Volume = BuoyancyData::CalVolume(Length, Diameter); //Automatically calculate the volume based off the other values
	FrontalArea = BuoyancyData::CalFrontalArea(Diameter); //Frontal area of the airship in meters squared
	LateralArea = BuoyancyData::CalLateralArea(Length, Diameter); //Lateral area of the airship in meters squared
	
	//Moved here since these should be updated on play after the editor has set the values in the BP
	// Turns off Unreal Physics Gravity system
	 if (AirshipMesh)
	 {
		AirshipMesh->SetEnableGravity(false);
	 	bool bIsGravityEnabled = AirshipMesh->IsGravityEnabled();
	 	UE_LOG(LogTemp, Warning, TEXT("Airship gravity enabled: %s"), bIsGravityEnabled ? TEXT("True") : TEXT("False"));
	 }
	//Set the airship as the default pawn
	SetDefaultPawn();
	
	// Populate Engines array with all attached engine components
	GetComponents(Engines);
	for (UEngines* Engine : Engines)
	{
		if (Engine)
		{
			UE_LOG(LogTemp, Log, TEXT("Engine found: %s"), *Engine->GetName());
			Engine->RegisterComponent(); // Ensure its act
			EngineMass = Engine->Mass * NumEngines; //gets the mass of the engines and passes it to EngineMass
		}
		else UE_LOG(LogTemp, Warning, TEXT("Engine not found"));
	}

	UpdateTotalMass(); //Done Here since Mass for engines isn't check until above
	//Add the mapping for the controls
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(AirshipMappingContext, 0);
		}
	}
}
//setup for the playinputs, the bind actions for each action is here with the function to be used and the BP function assoication
void AAirship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//control binding, minds the Blueprint slot with the function that actually does the action, the BP slot will have the Input Action assigned to it
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveZ, ETriggerEvent::Triggered, this, &AAirship::MoveZAxis);
		EnhancedInputComponent->BindAction(MoveNz, ETriggerEvent::Triggered, this, &AAirship::MoveNzAxis);
		EnhancedInputComponent->BindAction(MoveB, ETriggerEvent::Triggered, this, &AAirship::MoveBAxis);
		EnhancedInputComponent->BindAction(MoveX, ETriggerEvent::Triggered, this, &AAirship::MoveXAxis);
		EnhancedInputComponent->BindAction(ZeroPower,ETriggerEvent::Triggered, this, &AAirship::ZeroPowerAxis);
	}
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
		FVector GravityForce = FVector(0.0f, 0.0f, TotalMass * PhysicsConstants::GGravityOnEarth);
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
		FVector Power = TotalThrust * (Throttle / 100);

		//The netforce acting on the object, includes Bforce,Gforce,Drag,Engines and recoil
		FVector NetForce = BuoyantForce - GravityForce - DragForce + Power; //plus now since the minus is in the vector itself
		FVector Acceleration = NetForce / TotalMass;
		//verlet intergration
		FVector NewLocation = CurrentLocation + (CurrentLocation - PreviousPosition) + Acceleration * FMath::Square(DeltaTime);
		
		AddActorWorldOffset(NewLocation - CurrentLocation, true);
		Velocity = (NewLocation - PreviousPosition) / DeltaTime;
		PreviousPosition = CurrentLocation;

		// Apply smoothing (Exponential Smoothing)
		const float Alpha = 0.05f; // Smoothing factor
		SmoothedVelocity = Alpha * Velocity + (1.0f - Alpha) * SmoothedVelocity;
		SmoothedAcceleration = Alpha * Acceleration + (1.0f - Alpha) * SmoothedAcceleration;
		SmoothedNetForce = Alpha * NetForce + (1.0f - Alpha) * SmoothedNetForce;
		SmoothedDragForce = Alpha * DragForce + (1.0f - Alpha) * SmoothedDragForce;
		SmoothedThrust = Alpha * TotalThrust + (1.0f - Alpha) * SmoothedThrust;
		SmoothedPower = Alpha * Power + (1.0f - Alpha) * SmoothedPower;
		SmoothedBForce = Alpha * BuoyantForce + (1.0f - Alpha) * SmoothedBForce;
		
		//Debugging
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
			   -1,
			   0.0f,
			   FColor::Yellow,
			   FString::Printf(TEXT("Atmosphere Data:\n  - Temperature: %.4f\n  - Pressure: %.4f\n  - Density: %.4f\n"
				 "Engines:\n  - Throttle: (%.4f, %.4f, %.4f)\n  - Total Thrust: (%.4f, %.4f, %.4f)\n  - Power: (%.4f, %.4f, %.4f)\n"
				 "Mass:\n  - Total Mass: %.4f\n  - Engine Mass: %.4f\n"
				 "Forces:\n  - Net Force: (%.4f, %.4f, %.4f)\n  - Buoyant Force: (%.4f, %.4f, %.4f)\n"
				 "  - Gravity Force: (%.4f, %.4f, %.4f)\n  - Drag Force: (%.4f, %.4f, %.4f)\n"
				 "Physics:\n  - Altitude: %.4f\n  - Acceleration: (%.4f, %.4f, %.4f)\n  - Velocity: (%.4f, %.4f, %.4f)\n"),
				   Temperature, Pressure, Density,
				   Throttle.X, Throttle.Y, Throttle.Z,
				   SmoothedThrust.X, SmoothedThrust.Y, SmoothedThrust.Z,
				   SmoothedPower.X, SmoothedPower.Y, SmoothedPower.Z,
				   TotalMass, EngineMass,
				   SmoothedNetForce.X, SmoothedNetForce.Y, SmoothedNetForce.Z,
				   SmoothedBForce.X, SmoothedBForce.Y, SmoothedBForce.Z,
				   GravityForce.X, GravityForce.Y, GravityForce.Z,
				   SmoothedDragForce.X, SmoothedDragForce.Y, SmoothedDragForce.Z,
				   Altitude,
				   SmoothedAcceleration.X, SmoothedAcceleration.Y, SmoothedAcceleration.Z,
				   SmoothedVelocity.X, SmoothedVelocity.Y, SmoothedVelocity.Z));
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

void AAirship::SetFuelMass(float NewFuelMass)
{
	FuelMass = NewFuelMass;
}

void AAirship::SetCargoMass(float NewCargoMass)
{
	CargoMass = NewCargoMass;
}

void AAirship::SetBallastMass(float NewBallastMass)
{
	BallastMass = NewBallastMass;
}

void AAirship::GetEngineMass(UEngines* Engine)
{
	if (Engine)
	{
		UE_LOG(LogTemp, Log, TEXT("Engine found: %s"), *Engine->GetName());
		EngineMass = Engine->Mass * NumEngines; //gets the mass of the engines and passes it to EngineMass
	}
	else UE_LOG(LogTemp, Warning, TEXT("Engine not found"));
}

void AAirship::SetWeaponsMass(float NewWeaponsMass)
{
	WeaponMass = NewWeaponsMass;
}

void AAirship::UpdateTotalMass()
{
	TotalMass = DryMass + FuelMass + CargoMass + WeaponMass + BallastMass + 8; //8 is fixed just until enginemass problems sorted
}

void AAirship::MoveZAxis()
{
	Throttle.Z += 1.0f;
	Throttle.Z = FMath::Clamp(Throttle.Z, MinThrottle, MaxThrottle);
}

void AAirship::MoveNzAxis()
{
	Throttle.Z += -1.0f;
	Throttle.Z = FMath::Clamp(Throttle.Z, MinThrottle, MaxThrottle);
}

void AAirship::MoveBAxis()
{
	Throttle.X += 1.0f;
	Throttle.X = FMath::Clamp(Throttle.X, MinThrottle, MaxThrottle);
}

void AAirship::MoveXAxis()
{
	Throttle.X += -1.0f;
	Throttle.X = FMath::Clamp(Throttle.X, MinThrottle, MaxThrottle);
}

void AAirship::ZeroPowerAxis()
{
	Throttle = FVector(0,0,0); 
}



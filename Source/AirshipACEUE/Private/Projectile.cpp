// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    
	// Create the mesh component
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	if (ProjectileMesh)
	{
		ProjectileMesh->SetupAttachment(RootComponent);
	}
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (!ProjectileMovement)
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile %s has NO ProjectileMovementComponent!"), *GetName());
		return;
	}

	if (!ProjectileMovement->UpdatedComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile %s: ProjectileMovement has no UpdatedComponent!"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Projectile %s has a valid movement component."), *GetName());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::InitialiseProjectile(float Velocity, float Angle)
{
	if (!ProjectileMovement)
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile %s has no movement component!"), *GetName());
		return;
	}

	// Set velocity in the direction of the fire
	FVector LaunchVelocity = GetActorForwardVector() * Velocity;
	ProjectileMovement->Velocity = LaunchVelocity;
	ProjectileMovement->Activate();
	ProjectileMovement->InitialSpeed = 1.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
    
	UE_LOG(LogTemp, Log, TEXT("Projectile %s launched with velocity: %s"), *GetName(), *LaunchVelocity.ToString());
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	
}


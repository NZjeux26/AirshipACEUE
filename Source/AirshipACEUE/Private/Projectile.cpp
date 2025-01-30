// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::InitialiseProjectile(float Velocity, float Angle)
{
	if (!ProjectileMovement) return;

	// Get forward direction in 2D space (ignoring Y)
	FVector LaunchDirection = GetActorForwardVector();
	LaunchDirection.Y = 0; // Lock to 2D

	// Apply velocity in X-Z plane
	FVector LaunchVelocity = LaunchDirection * Velocity;

	ProjectileMovement->Velocity = LaunchVelocity;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	
}


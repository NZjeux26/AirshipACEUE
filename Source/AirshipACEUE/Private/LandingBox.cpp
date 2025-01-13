// Fill out your copyright notice in the Description page of Project Settings.


#include "LandingBox.h"

#include "GameFramework/Character.h"


// Sets default values
ALandingBox::ALandingBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALandingBox::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ALandingBox::OnOverlapBegin);
}

// Called every frame
void ALandingBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALandingBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// Ensure the overlapping actor is valid and is the player character
	if (OtherActor && OtherActor != this)
	{
		if (ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player has entered the end zone!"));
		}
	}
}


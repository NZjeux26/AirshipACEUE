// Fill out your copyright notice in the Description page of Project Settings.


#include "LandingBox.h"

#include "Airship.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALandingBox::ALandingBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Enables collision components
	if (GetCollisionComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Collision Component is valid!"));
		GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetCollisionComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
		GetCollisionComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
		GetCollisionComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Collision Component is NULL!"));
	}
}

// Called when the game starts or when spawned
void ALandingBox::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ALandingBox::OnOverlapBegin);
	DrawDebugBox(GetWorld(), GetActorLocation(), GetCollisionComponent()->Bounds.BoxExtent, FColor::Green, true, 5.0f);
}
// Called every frame
void ALandingBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//landing box trigger, checks if an pawn crosses, if that pawn is an airship and triggers something.
void ALandingBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Something overlapped with the LandingBox!"));

	if (OtherActor && OtherActor != this)
	{
		// Log the name of the overlapping actor
		UE_LOG(LogTemp, Warning, TEXT("Something overlapped with the LandingBox: %s"), *OtherActor->GetName());

		// Check if the overlapping actor is an Airship
		if (AAirship* Airship = Cast<AAirship>(OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("An Airship has entered the LandingBox!"));
			UGameplayStatics::OpenLevel(this, FName("GameEndLevel"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The overlapping actor is not an Airship."));
		}
	}
}


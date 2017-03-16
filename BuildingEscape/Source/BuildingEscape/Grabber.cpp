// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

// Blanck MACRO. Just to remind us what is going on
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// UE_LOG is expecting a TCHAR array, and the * effectively converts the string to this type
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for DUTY!!!"));
	ActorController = GetWorld()->GetFirstPlayerController();
	Reach = 100.0f;
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	ActorController->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, // The calling function will change this variable
		OUT PlayerViewPointRotation
	);
	// Prints Location and location to the screen
	// REMEMBER!!!! WE NEED TO DEREFERENCE BY ADDING A STAR TO THE PARAMETERS OF THE FORMATING STRING!!!!
	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), 
		*PlayerViewPointLocation.ToString(), 
		*PlayerViewPointRotation.ToString()
	);*/

	// Draw a red trace in the world to visual
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Line-trace (AKA Ray-cast) out to reach distance. Laser until obstacle
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// Log what we hit
	AActor* ActorHitted = Hit.GetActor();
	if (ActorHitted) // Check for null
	{
		FString ActorsName = ActorHitted->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"),
				*ActorsName
			);
	}
	
}


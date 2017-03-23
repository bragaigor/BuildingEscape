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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// UE_LOG is expecting a TCHAR array, and the * effectively converts the string to this type
	// UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for DUTY!!!"));
	ActorController = GetWorld()->GetFirstPlayerController();
	Reach = 100.0f;

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent() {
	/// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle != nullptr)
	{
		// Physics handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Physics Component."),
			*GetOwner()->GetName()
		);
	}
}

void UGrabber::SetupInputComponent()
{
	/// Look for attached Input Component (only appears at run time)
	// InputComponent = GetOwner()->InputComponent; 
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>(); // both ways work

	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found input component."));
		/// Bind the input axis
		// Parameters:
		// 1: Name of ProjectSettings/Engine/Input ActionMappings name (Action name)
		// 2: Enum from input event (there's another one for release)
		// 3: Reference to the component itself (Grabber)
		// 4: Method (function) name, function to be performed when key is pressed in this case
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Input Component."),
			*GetOwner()->GetName()
		);
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed."));

	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	/// Auto will set the variable to the type that the function returns. In this case is a FHitResult
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // Gets the mesh in our case 
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit)
	{
		// Attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // No bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // Allow rotation
		);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released."));
	// TODO release physics handle
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// If the physics handle is atached 
	if (PhysicsHandle->GrabbedComponent)
	{
		// Sets the end of a line trace in the world according to the Pawn location
		FVector LineTraceEnd = GetLineTraceEnd();
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get player's start position for line trace
	FVector PlayerViewPointLocation = GetLineTraceStart();
	
	// Draw a red trace in the world to visual
	// Calculates the where the LINE TRACE ends ( * Reach)
	FVector LineTraceEnd = GetLineTraceEnd();
		//DrawDebugLine(
		//	GetWorld(),
		//	PlayerViewPointLocation,
		//	LineTraceEnd,
		//	FColor(255, 0, 0),
		//	false,
		//	0.f,
		//	0.f,
		//	10.f
		//);

	// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Line-trace (AKA Ray-cast) out to reach distance. Laser until obstacle
	FHitResult Hit;
	// Detects objects when the actor gets close enough to them.
	// It only detects objects of type PhysicsBody
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
	return Hit;
}

FVector UGrabber::GetLineTraceStart() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	ActorController->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, // The calling function will change this variable
		OUT PlayerViewPointRotation
	);

	// Sets the end of a line trace in the world according to the Pawn location
	return PlayerViewPointLocation;
}

// Gets player view point
FVector UGrabber::GetLineTraceEnd() const
{
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

	// Sets the end of a line trace in the world according to the Pawn location
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

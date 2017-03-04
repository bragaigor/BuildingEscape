// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	// Find creator actor
	Owner = GetOwner();
	// A pawn is a actor that's why we can do it
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

void UOpenDoor::OpenDoor()
{
	
	// Create a rotator.
	// Parameters.
	// 1st Pitch:
	// 2nd Yaw:
	// 3rd Roll:
	// FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);
	// Find Z rotation 
	// Owner->SetActorRotation(NewRotation);
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	isDoorOpenned = true;
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle-90.f, 0.0f));
	isDoorOpenned = false;
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Pool the Trigger Volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	// Check if it is time to close the door
	if (isDoorOpenned) {
		if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay) {
			CloseDoor();
		}
	}
	
}


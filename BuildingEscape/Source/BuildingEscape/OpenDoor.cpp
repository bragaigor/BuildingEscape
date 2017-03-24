// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	// Find creator actor
	Owner = GetOwner();
	// A pawn is a actor that's why we can do it
	// ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName())
	}
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

	// Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	OnOpen.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	// Owner->SetActorRotation(FRotator(0.0f, OpenAngle-90.f, 0.0f));
	OnClose.Broadcast();
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (GetTotalMassOfActorOnPlate() > TriggerMass) { 
		OpenDoor();
	}
	else
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorOnPlate() {
	float TotalMass = 0.f;

	//Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	// This method returns an array of overlapping actor that will be stored in OverlappingActors
	if (!PressurePlate) { return -1; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses
	if (OverlappingActors.IsValidIndex(0))
	{
		for (const auto* thisActor : OverlappingActors) {
			UPrimitiveComponent* PrimitiveComponent = thisActor->FindComponentByClass<UPrimitiveComponent>();
			TotalMass += PrimitiveComponent->GetMass();
			UE_LOG(LogTemp, Warning, TEXT("Actor in pressure plate is %s."),
				*thisActor->GetName()
			);
		}
	}
	return TotalMass;
}


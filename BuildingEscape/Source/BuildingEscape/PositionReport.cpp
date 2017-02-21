// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "PositionReport.h"


// Sets default values for this component's properties
UPositionReport::UPositionReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UPositionReport::BeginPlay()
{
	Super::BeginPlay();

	AActor* someActor = GetOwner();
	FString ObjectName = someActor->GetName();
	FString ObjectPos = "X = " + FString::SanitizeFloat(someActor->GetActorLocation().X) + 
						FString(", Y = ") + FString::SanitizeFloat(someActor->GetActorLocation().Y) + 
						FString(", Z = ") + FString::SanitizeFloat(someActor->GetActorLocation().Z);
	// or
	FString ObjectPos2 = someActor->GetTransform().GetLocation().ToString();
	// or
	FString ObjectPos3 = someActor->GetActorLocation().ToString();
	
	// MACRO. Log color
	// Warning: Yellow
	// Log: Grey
	// Error: Red
	// UE_LOG is expecting a TCHAR array, and the * effectively converts the string to this type
	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *ObjectName, *ObjectPos3);
	
}


// Called every frame
void UPositionReport::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}


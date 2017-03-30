// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "SoundEffectObject.h"


// Sets default values for this component's properties
USoundEffectObject::USoundEffectObject()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USoundEffectObject::BeginPlay()
{
	Super::BeginPlay();

	// Find creator actor
	Owner = GetOwner();

	if (Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("My name is %s"), *Owner->GetName())
	}

	// TODO Add event so that the actor plays a sound everytime it hits something 
	
}


// Called every frame
void USoundEffectObject::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}


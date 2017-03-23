// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
private:
	// Makes it available in unreal editor, however it is not editable
	UPROPERTY(VisibleAnywhere)
	float openAngle = 90.0f;

	// Makes it available in unreal editor, and it is editable 
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.0f;

	float LastDoorOpenTime;
	float OpenAngle = 0.0f;
	bool isDoorOpenned = false;

	// Makes it available in unreal editor, and it is editable 
	// UPROPERTY(EditAnywhere)
	// AActor* ActorThatOpens;
	AActor* Owner; // The owning door

	// Returns total mass in kg in pressure plate
	float GetTotalMassOfActorOnPlate();
};

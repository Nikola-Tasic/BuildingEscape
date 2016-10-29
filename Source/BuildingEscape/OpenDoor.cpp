// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner(); //pointer na ownera kao AActor, njegovu nadklasu
	//nub, bio sam duplo deklarisao Owner, vec ga imam u .h fajl, sad radi
}

void UOpenDoor::OpenDoor()
{
	FRotator NewRotation = FRotator(0.f, OpenAngle, 0.f); //floutingpoiiiiiint (OpenAngle definisan u .h)
													  //FRotator prima pitch (kao pedala), yaw (kao vrata), i roll (kao volan)
													  //svaka cast samom sebi za objasnjenje^
	Owner->SetActorRotation(NewRotation);
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f)); //skraceno, ostavio sam celo gore u OpenDoor
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//izvrsava se svakog frejma
	// ako je (AActor) ActorThatOpens unutar (TriggerVolume) PressurePlate (overlapuju se), pozovi OpenDoor
	if(IsValid(PressurePlate) && IsValid(ActorThatOpens)) //NECE BEZ OVO DA RADI, NE ZNAM ZASTO, CRASHUJE EDITOR
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds(); //pamti vreme kad su otvorena vrata
	}

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
	

}


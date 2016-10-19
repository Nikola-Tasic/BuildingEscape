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
}

void UOpenDoor::OpenDoor()
{
	AActor* Owner = GetOwner(); //pointer na ownera kao AActor, njegovu nadklasu
	FRotator NewRotation = FRotator(0.f, -60.f, 0.f); //floutingpoiiiiiint
													  //FRotator prima pitch (kao pedala), yaw (kao vrata), i roll (kao volan)
													  //svaka cast samom sebi za objasnjenje^
	Owner->SetActorRotation(NewRotation);
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
	}

}


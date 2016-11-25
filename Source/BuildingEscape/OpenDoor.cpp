// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner(); //pointer na ownera kao AActor, njegovu nadklasu
	//nub, bio sam duplo deklarisao Owner, vec ga imam u .h fajl, sad radi
}

// Ne radi nista, prebaceno dole u tick, ostavljam da znam sta je bilo
void UOpenDoor::OpenDoor()
{
	// OnOpenRequest.Broadcast(); 
	// FRotator NewRotation = FRotator(0.f, OpenAngle, 0.f);  //floutingpoiiiiiint (OpenAngle definisan u .h)
		    											      //FRotator prima pitch (kao pedala), yaw (kao vrata), i roll (kao volan)
															  //svaka cast samom sebi za objasnjenje^
	//Owner->SetActorRotation(NewRotation);
}

// Ne radi nista, prebaceno dole u tick, ostavljam da znam sta je bilo
void UOpenDoor::CloseDoor()
{
	//Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f)); //skraceno, ostavio sam celo gore u OpenDoor
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	
	if(IsValid(PressurePlate)) //NECE BEZ OVO DA RADI, NE ZNAM ZASTO, CRASHUJE EDITOR
	// Ako objekti na presure plate imaju vecu masu od TriggerMass, otvori vrata
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

// Vrati ukupnu tezinu svih objekata na pressure plate
float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Pronaði sve objekte koji su na pressure plate
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors); // KO JE PRAVIO OVAJ UNREAL ENGINE, SETTER SE ZOVE GET

	// Saberi mase svih objekta
	for (const auto& Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT //ovo ustvari ne radi nista


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FVector PVPLocation;
	FRotator PVPRotation;

	// OVAJ RETARDIRANI GETER USTVARI NE GETUJE NIŠTA JER JE VOID, I USTVARI POSTAVLJA VREDNOST PROSLEÐENIM PARAMETRIMA
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PVPLocation, OUT PVPRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Position: %s"), *PVPLocation.ToString(), *PVPRotation.ToString()) //macro je, ne mora ; 

	FVector LineTraceEnd = PVPLocation + PVPRotation.Vector() * Reach;

	// Draw red line that represents the direction the pawn is looking at
	DrawDebugLine(
		GetWorld(),
		PVPLocation,
		LineTraceEnd, 
		FColor(255, 0, 0),
		false, // no persist, brisi slobodno
		0.f, // bez persist mu ne treba ni life time
		0.f, // priority
		10.f // thickness
		);


}


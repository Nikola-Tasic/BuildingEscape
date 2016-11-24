#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT //ovo ustvari ne radi nista

// Konstruktor
UGrabber::UGrabber()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Pronaði dodati PhysicsHandle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>(); //jeej generici
	if (!PhysicsHandle) //uvek radi ovo za svaki slucaj da ne crashuje
	{
		UE_LOG(LogTemp, Error, TEXT("% missing physics handle component"), *GetOwner()->GetName());
	}
}

// Pronaði dodati InputComponent koji se pojavljuje u runtime
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found!"));
		//Bind input action. "Grab" je postavljen za ime input akcije u UE4 Engine Input
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("% missing input component"), *GetOwner()->GetName());
	}
}

// Nacrtaj crvenu liniju koja pokazuje gde actor gleda
void UGrabber::DrawRedLine(FVector PVPLocation, FVector LineTraceEnd)
{
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

// Uhvati objekat
void UGrabber::Grab()
{
	///UE_LOG(LogTemp, Warning, TEXT("Grab key pressed!")); //haha tipka je utipkana, nene551
    // Uradi line trace i pronaði actore sa postavljenom kolizijom
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	// Ako doðe do kolizije, zakaci physics handle
	if (ActorHit != nullptr)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // dozvoli rotaciju
		);
	}
}

// Pusti objekat
void UGrabber::Release()
{
	///UE_LOG(LogTemp, Warning, TEXT("Grab key released!")); 
	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FVector PVPLocation;
	FRotator PVPRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PVPLocation, OUT PVPRotation);
	FVector LineTraceEnd = PVPLocation + PVPRotation.Vector() * Reach;

	// Ako je dodat physics handle, pomeri objekat
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

}

// Vraca hit za prvi objekat koji moze da uhvati
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PVPLocation;
	FRotator PVPRotation;

	// OVAJ RETARDIRANI GETER USTVARI NE GETUJE NIŠTA JER JE VOID, I USTVARI POSTAVLJA VREDNOST PROSLEÐENIM PARAMETRIMA
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PVPLocation, OUT PVPRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Position: %s"), *PVPLocation.ToString(), *PVPRotation.ToString()) //macro je, ne mora ; 

	FVector LineTraceEnd = PVPLocation + PVPRotation.Vector() * Reach;

	//LineTrace (Ray-Cast). ObjectType je PhysicsBody, WorldStatic itd.

	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); //GetOwner da mi ignorisalo koliziju sa samim sobom

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PVPLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), //gledaj samo PhysicsBody
		TraceParameters
	);

	AActor* HitActor = Hit.GetActor();
	if (HitActor) //za svaki slucaj, da ne crashuje
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hi %s"), *(HitActor->GetName()));
	}

	return Hit;
}
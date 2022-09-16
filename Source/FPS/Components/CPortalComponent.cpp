#include "Components/CPortalComponent.h"
#include "Global.h"
#include "Objects/CPortal.h"
#include "Objects/CPortalWall.h"

UCPortalComponent::UCPortalComponent()
{

}

void UCPortalComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCPortalComponent::SpawnPortalAlongVector(FVector StartLocation, FVector Direction, bool IsPortalA)
{
	FHitResult result;
	bool trace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartLocation, StartLocation + (Direction * MaxSpawnDistance), ObjectTypes, 0, Actors, EDrawDebugTrace::ForDuration, result, 1);
	CheckFalse(trace);
	ACPortal* newPortal;
	ACPortalWall* wall = Cast<ACPortalWall>(result.GetActor());
	CheckNull(wall);
	FVector loc = result.Location + UKismetMathLibrary::Normal(result.TraceStart - result.TraceEnd);
	newPortal = wall->TryAddPortal(loc, IsPortalA);
	CheckNull(newPortal);
	SwapPortals(IsPortalA ? PortalA : PortalB, newPortal);
	if (PortalA && PortalB)
	{
		PortalB->LinkPortal(PortalA);
		PortalA->LinkPortal(PortalB);
	}
}

void UCPortalComponent::SwapPortals(ACPortal*& Portal, ACPortal* NewPortal)//내용물을 지우고 주소값을 바꿔주기위함
{
	if (Portal)Portal->Destroy();
	Portal = NewPortal;
}

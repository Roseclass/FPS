#include "Objects/CInteract_IK.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

ACInteract_IK::ACInteract_IK()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent(this, &Mesh, "Mesh");
	CHelpers::CreateComponent(this, &Box, "Box", Mesh);
}

void ACInteract_IK::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACInteract_IK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


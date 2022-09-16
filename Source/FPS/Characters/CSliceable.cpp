#include "Characters/CSliceable.h"
#include "Global.h"
#include "Components/CSliceComponent.h"

ACSliceable::ACSliceable()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCSliceComponent>(this, &Slice, "Slice");
}
void ACSliceable::BeginPlay()
{
	Super::BeginPlay();	
}

void ACSliceable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

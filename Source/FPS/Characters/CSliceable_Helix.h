#pragma once

#include "CoreMinimal.h"
#include "Characters/CSliceable.h"
#include "CSliceable_Helix.generated.h"

UCLASS()
class FPS_API ACSliceable_Helix : public ACSliceable
{
	GENERATED_BODY()
//property
private:
protected:
public:

//function
private:
protected:
public:
	ACSliceable_Helix();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void DoSlice(FName BoneName, FVector PlanePos, EProcMeshSliceCapOption Option, FVector HitNormal) override;
};

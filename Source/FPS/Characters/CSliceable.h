#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KismetProceduralMeshLibrary.h"
#include "CSliceable.generated.h"

UCLASS()
class FPS_API ACSliceable : public ACharacter
{
	GENERATED_BODY()
//property
private:
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCSliceComponent* Slice;
public:

//function
private:
protected:
	virtual void BeginPlay() override;

public:
	ACSliceable();
	virtual void Tick(float DeltaTime) override;

	virtual void DoSlice(FName BoneName, FVector PlanePos, EProcMeshSliceCapOption Option, FVector HitNormal) {};
};

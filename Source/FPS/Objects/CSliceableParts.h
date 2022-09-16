// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KismetProceduralMeshLibrary.h"
#include "CSliceableParts.generated.h"

UCLASS()
class FPS_API ACSliceableParts : public AActor
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UProceduralMeshComponent* ProcMesh;

	UPROPERTY(EditAnywhere)
		class UMaterialInterface* PlaneMaterial;

protected:
public:

//function
private:
protected:
	virtual void BeginPlay() override;
public:
	ACSliceableParts();
	virtual void OnConstruction(const FTransform& Transform) override;
	void Slice(FVector PlanePos, FVector PlaneVector, bool CreatOtherHalf, EProcMeshSliceCapOption Option);
};

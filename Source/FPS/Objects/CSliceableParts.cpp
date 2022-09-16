#include "Objects/CSliceableParts.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACSliceableParts::ACSliceableParts()
{
	CHelpers::CreateComponent(this, &Scene, "Scene");
	CHelpers::CreateComponent(this, &StaticMesh, "StaticMesh", Scene);
	CHelpers::CreateComponent(this, &ProcMesh, "ProcMesh", Scene);
	
	StaticMesh->SetVisibility(false);
	StaticMesh->SetCollisionProfileName("NoCollision");

	ProcMesh->SetSimulatePhysics(true);
	ProcMesh->bUseComplexAsSimpleCollision = false;
}

void ACSliceableParts::OnConstruction(const FTransform& Transform)
{
	UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(StaticMesh, 0, ProcMesh, false);
}

void ACSliceableParts::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACSliceableParts::Slice(FVector PlanePos,FVector PlaneVector,bool CreatOtherHalf,EProcMeshSliceCapOption Option)
{
	CLog::Print("IN");
	UProceduralMeshComponent* outProcMesh = nullptr;

	UMaterialInstanceDynamic* dynMaterial = UMaterialInstanceDynamic::Create(Cast<UMaterialInstanceConstant>(PlaneMaterial), this);

	UKismetProceduralMeshLibrary::SliceProceduralMesh
	(
		ProcMesh,
		PlanePos,
		PlaneVector,
		CreatOtherHalf,
		outProcMesh,
		Option,
		PlaneMaterial
	);

	outProcMesh->SetSimulatePhysics(true);
	outProcMesh->AddImpulse(FVector(1000.0f, 1000.0f, 1000.0f), NAME_None, true);


}

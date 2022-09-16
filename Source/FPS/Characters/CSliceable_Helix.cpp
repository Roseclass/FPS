#include "Characters/CSliceable_Helix.h"
#include "Global.h"
#include "Characters/CSliceable.h"
#include "Components/CSliceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "PhysicsEngine/PhysicsAsset.h"

ACSliceable_Helix::ACSliceable_Helix()
{
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh,"SkeletalMesh'/Game/Enemies/Minion/Helix/Mesh/Helix_Slice_Skeletal.Helix_Slice_Skeletal'");
	GetMesh()->SkeletalMesh = mesh;
	Slice->SetUp(mesh->GetPhysicsAsset());
	Slice->SetMesh(GetMesh());
}

void ACSliceable_Helix::OnConstruction(const FTransform& Transform)
{
	for (auto i : GetMesh()->SkeletalMesh->GetPhysicsAsset()->SkeletalBodySetups)
	{
		FName bonName = i->BoneName;
		if (!Slice->IsInMap(bonName))continue;
		UStaticMeshComponent* mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
		UProceduralMeshComponent* upper= NewObject<UProceduralMeshComponent>(this, UProceduralMeshComponent::StaticClass());
		UProceduralMeshComponent* lower = NewObject<UProceduralMeshComponent>(this, UProceduralMeshComponent::StaticClass());

		UStaticMesh* staticMesh = Slice->GetMesh(i->BoneName);

		mesh->SetVisibility(false);
		mesh->SetCollisionProfileName("NoCollision");
		if(staticMesh)mesh->SetStaticMesh(staticMesh);

		//proc->SetSimulatePhysics(true);
		//proc->bUseComplexAsSimpleCollision = false;

		mesh->RegisterComponent();
		upper->RegisterComponent();
		lower->RegisterComponent();

		UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(mesh, 0, upper, false);
		UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(mesh, 0, lower, false);

		FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, 1);

		mesh->AttachToComponent(GetMesh(), rule, Slice->GetSocketName(i->BoneName));
		upper->AttachToComponent(GetMesh(), rule, Slice->GetSocketName(i->BoneName));
		lower->AttachToComponent(GetMesh(), rule, Slice->GetSocketName(i->BoneName));

		Slice->SetUpperProcMesh(bonName, upper);
		Slice->SetLowerProcMesh(bonName, lower);
		Slice->SetMap();
	}
}

void ACSliceable_Helix::DoSlice(FName BoneName, FVector PlanePos, EProcMeshSliceCapOption Option, FVector HitNormal)
{
	if (!Slice->IsInMap(BoneName))return;
	Slice->DoSlice(BoneName, PlanePos, Option, HitNormal);
}

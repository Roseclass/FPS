#include "Components/CSliceComponent.h"
#include "Global.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

UCSliceComponent::UCSliceComponent()
{

}

void UCSliceComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UCSliceComponent::SetUp(UPhysicsAsset* InPhysicsAsset)
{
	if (InPhysicsAsset)
		for (auto i : InPhysicsAsset->SkeletalBodySetups)
			StructMap.Add(i->BoneName, FSliceStruct());
}

void UCSliceComponent::SetMesh(USkeletalMeshComponent* InMesh)
{
	Mesh = InMesh;
}

void UCSliceComponent::SetMap()
{
	int i = 0;
}

void UCSliceComponent::MovePartsTo(FName InBoneName)
{
	//UProceduralMeshComponent* temp = *PartsMap.Find(InBoneName);
	//CheckNull(temp);
	//FName name = *SocketMap.Find(InBoneName);
	//CheckTrue(name == NAME_None);
	//ACharacter* character = Cast<ACharacter>(GetOwner());
	//CheckNull(character);
	//FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative,1);
	//temp->AttachToComponent(character->GetMesh(), rule, name);
}

void UCSliceComponent::SetUpperProcMesh(FName InBoneName, UProceduralMeshComponent* InComponent)
{
	StructMap[InBoneName].Upper = InComponent;
}

void UCSliceComponent::SetLowerProcMesh(FName InBoneName, UProceduralMeshComponent* InComponent)
{
	StructMap[InBoneName].Lower = InComponent;
	StructMap[InBoneName].Lower->SetHiddenInGame(1);
}

void UCSliceComponent::DoSlice(FName BoneName, FVector PlanePos, EProcMeshSliceCapOption Option,FVector HitNormal)
{
	if (!StructMap[BoneName].Sliced)
	{
		FirstSlice(BoneName, PlanePos, Option, HitNormal);
		return;
	}
	//UProceduralMeshComponent* outProcMesh = nullptr;	
	//UMaterialInstanceDynamic* dynMaterial = UMaterialInstanceDynamic::Create(Cast<UMaterialInstanceConstant>(PlaneMaterial), this);
	
	//ProcMesh->bUseComplexAsSimpleCollision = false;
	//ProcMesh->SetSimulatePhysics(true);
	//GetMesh()->BreakConstraint(PlaneVector * 500.0f, PlanePos, BoneName);
	
	//UKismetProceduralMeshLibrary::SliceProceduralMesh
	//(
	//	ProcMesh,
	//	PlanePos,
	//	PlaneVector,
	//	CreatOtherHalf,
	//	outProcMesh,
	//	Option,
	//	PlaneMaterial
	//);
	//
	//if (outProcMesh)
	//{
	//	outProcMesh->bUseComplexAsSimpleCollision = false;
	//	outProcMesh->SetSimulatePhysics(true);
	//	outProcMesh->AddImpulse(FVector(1000.0f, 1000.0f, 1000.0f), NAME_None, true);
	//}
}
void UCSliceComponent::FirstSlice(FName BoneName, FVector PlanePos, EProcMeshSliceCapOption Option, FVector HitNormal)
{
	StructMap[BoneName].Sliced = 1;

	FVector up = Mesh->GetSocketLocation(StructMap[BoneName].SocketName);
	FVector low = Mesh->GetSocketLocation(StructMap[BoneName].LowerSocketName);
	//FVector dir = UKismetMathLibrary::Normal(low - up);
	//
	//float length = UKismetMathLibrary::Vector_Distance(up, low);
	//float dist = abs(UKismetMathLibrary::Dot_VectorVector(dir, PlanePos-up));
	//
	//FVector point = UKismetMathLibrary::VLerp(up, low, dist / length);
	//
	//up = UKismetMathLibrary::Normal(up - point);
	//low = UKismetMathLibrary::Normal(low - point);

	FVector normal = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromXZ(UKismetMathLibrary::Normal(low - up),HitNormal));

	UProceduralMeshComponent* outProcMesh = nullptr;
	UKismetProceduralMeshLibrary::SliceProceduralMesh(StructMap[BoneName].Upper, PlanePos, -normal, 0, outProcMesh, Option, PlaneMaterial);
	UKismetProceduralMeshLibrary::SliceProceduralMesh(StructMap[BoneName].Lower, PlanePos, normal, 0, outProcMesh, Option, PlaneMaterial);

	StructMap[BoneName].Lower->SetHiddenInGame(0);
	FAttachmentTransformRules f = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative,1);
	StructMap[BoneName].Lower->AttachToComponent(Mesh, f, StructMap[BoneName].LowerSocketName);
	if (StructMap[BoneName].ConstraintName != NAME_None)
	{
		Mesh->BreakConstraint(-HitNormal * 100.0f, PlanePos, StructMap[BoneName].ConstraintName);
		//Mesh->BreakConstraint(normal * 100.0f, PlanePos, StructMap[BoneName].ConstraintName);
		//Mesh->SetAllBodiesBelowSimulatePhysics(StructMap[BoneName].ConstraintName, 1, 1);
	}
}

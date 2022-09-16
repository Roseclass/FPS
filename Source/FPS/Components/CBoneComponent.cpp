#include "Components/CBoneComponent.h"
#include "Global.h"

UCBoneComponent::UCBoneComponent()
{
}

void UCBoneComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UCBoneComponent::ReadBoneData()
{
	if (PhysicsAsset)
		for (auto i : PhysicsAsset->SkeletalBodySetups)
			BoneMap.Add(i->BoneName, 1.0f);	
}

void UCBoneComponent::SetCurrentHittedBone(FName InBoneName)
{
	Current = InBoneName;
}
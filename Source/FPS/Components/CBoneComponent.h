#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBoneComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UCBoneComponent : public UActorComponent
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(EditAnywhere)
		TMap<FName, float> BoneMap;

	FName Current;
protected:
public:
	UPROPERTY(EditAnywhere)
		class UPhysicsAsset* PhysicsAsset;

//function
private:
protected:
	virtual void BeginPlay() override;

public:
	UCBoneComponent();
	
	void ReadBoneData();
	void SetCurrentHittedBone(FName InBoneName);
	FORCEINLINE float GetRate(FName InBoneName) { return BoneMap.Contains(InBoneName) ? BoneMap[InBoneName] : 1.0f;}
	FORCEINLINE float GetCurrentHittedBoneRate() { return BoneMap.Contains(Current) ? BoneMap[Current] : 0.0f ; }
};

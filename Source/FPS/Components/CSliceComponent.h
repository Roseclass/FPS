#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "CSliceComponent.generated.h"

USTRUCT(BlueprintType)
struct FSliceStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FName SocketName;

	UPROPERTY(EditAnywhere)
		FName LowerSocketName;

	UPROPERTY(EditAnywhere)
		FName ConstraintName;

	UPROPERTY(EditAnywhere)
		class UStaticMesh* Mesh;

	class UProceduralMeshComponent* Upper;
	UProceduralMeshComponent* Lower;
	bool Sliced = false;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UCSliceComponent : public UActorComponent
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(EditAnywhere)
		TMap<FName, FSliceStruct> StructMap;

	UPROPERTY(EditAnywhere)
		class UMaterialInterface* PlaneMaterial;

	class USkeletalMeshComponent* Mesh;
protected:
public:

//function
private:
	void FirstSlice(FName BoneName, FVector PlanePos, EProcMeshSliceCapOption Option, FVector HitNormal);
protected:
	virtual void BeginPlay() override;
public:	
	UCSliceComponent();		
	void SetUp(class UPhysicsAsset* InPhysicsAsset);
	void SetMesh(USkeletalMeshComponent* InMesh);
	void SetMap();
	void DoSlice(FName BoneName, FVector PlanePos, EProcMeshSliceCapOption Option, FVector HitNormal);

	void MovePartsTo(FName InBoneName);
	bool IsInMap(FName InBoneName) { return StructMap.Contains(InBoneName); }

	FORCEINLINE FName GetSocketName(FName InBoneName) { return StructMap[InBoneName].SocketName; }
	FORCEINLINE FName GetConstraintName(FName InBoneName) { return StructMap[InBoneName].ConstraintName; }
	FORCEINLINE UStaticMesh* GetMesh(FName InBoneName) { return StructMap[InBoneName].Mesh; }
	FORCEINLINE UProceduralMeshComponent* GetUpperProcMesh(FName InBoneName) { return StructMap[InBoneName].Upper; }
	FORCEINLINE UProceduralMeshComponent* GetLowerProcMesh(FName InBoneName) { return StructMap[InBoneName].Lower; }
	
	void SetUpperProcMesh(FName InBoneName, UProceduralMeshComponent* InComponent);
	void SetLowerProcMesh(FName InBoneName, UProceduralMeshComponent* InComponent);
};

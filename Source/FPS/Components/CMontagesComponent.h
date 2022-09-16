#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CStateComponent.h"
#include "Engine/DataTable.h"
#include "CMontagesComponent.generated.h"


USTRUCT(BlueprintType)
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EStateType Type;

	UPROPERTY(EditAnywhere)
		TArray<class UAnimMontage*> AnimMontages;

	UPROPERTY(EditAnywhere)
		TArray<float> PlayRates;

	UPROPERTY(EditAnywhere)
		TArray<FName> StartSections;

	UPROPERTY(EditAnywhere)
		TArray<bool> bCanMoves;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DataTable;

	FMontageData* Datas[(int32)EStateType::Max];

protected:
public:
//function
private:
	void PlayAnimMontage(EStateType InStateType, int32 InIndex);
	 
protected:
	virtual void BeginPlay() override;

public:
	UCMontagesComponent();

	void PlayRoll(int32 InIndex=0);
	void PlayHitted(int32 InIndex=0);
	void PlayDead(int32 InIndex=0);
	void PlayEmotion(int32 InIndex=0);

	float GetPlayLength(int32 DataIndex, int32 InIndex);
};

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actions/CAction.h"
#include "CActionData.generated.h"

UCLASS()
class FPS_API UCActionData : public UDataAsset
{
	GENERATED_BODY()
//property
private:
protected:
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACEquipment> EquipmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor EquipmentColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACDoAction> DoActionClass;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipmentData ReloadData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionWidgetData> DoActionWidgetDatas;

//function
private:
	FString GetLabelName(class ACharacter* InOwnerCharacter, FString InName);

protected:
public:
	void BeginPlay(class ACharacter* InOwnerCharacter, class UCAction** OutAction);

};

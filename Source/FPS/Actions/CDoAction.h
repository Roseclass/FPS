#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionData.h"
#include "CDoAction.generated.h"

UCLASS()
class FPS_API ACDoAction : public AActor
{
	GENERATED_BODY()
//property
private:
protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

	TArray<FDoActionData> Datas;
	FEquipmentData ReloadData;
	TArray<FDoActionWidgetData> WidgetDatas;


	const bool* bEquipped;

public:

//function
private:
protected:
	virtual void BeginPlay() override;

public:
	ACDoAction();

	virtual void Tick(float DeltaTime) override;

	virtual void DoAction() {}
	virtual void Begin_DoAction() {}
	virtual void End_DoAction() {}

	virtual void DoSkill(int32 InIdx) {}
	virtual void Begin_DoSkill() {}
	virtual void End_DoSkill() {}

	virtual void Reload() {}
	virtual void Begin_Reload() {}
	virtual void End_Reload() {}

	virtual void Abort() {}

	virtual void OnAim() {}
	virtual void OffAim() {}

	virtual bool CanSelect() { return 0; }

	virtual void OnSelectMode() {}
	virtual void OffSelectMode() {}

	FORCEINLINE void SetData(TArray<FDoActionData> InDatas) { Datas = InDatas; }
	FORCEINLINE void SetReloadData(FEquipmentData InDatas) { ReloadData = InDatas; }
	FORCEINLINE void SetWidgetData(TArray<FDoActionWidgetData> InDatas) { WidgetDatas = InDatas; }
	FORCEINLINE void SetEquipped(const bool* InEquipped) { bEquipped = InEquipped; }

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {};

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {};

};

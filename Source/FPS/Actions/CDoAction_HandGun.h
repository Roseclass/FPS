#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "Components/CActionComponent.h"
#include "Widgets/CUserWidget_Select.h"
#include "CDoAction_HandGun.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSelectModeOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSelectModeOff);

UCLASS()
class FPS_API ACDoAction_HandGun : public ACDoAction
{
	GENERATED_BODY()
//property
private:
	UPROPERTY()
		class UCAim* Aim;

	class UCActionComponent* Action;
	class UCUserWidget_WeaponInfo* InfoWidget;
	int32 MaxCount=5;
	int32 Count=5;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UCUserWidget_Select> SelectWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UCUserWidget_SkillSlot> SlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TArray <FWidgetTransform> WidgetTransforms;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TArray <int32> WidgetDataIndices;//슬롯에 들어가있는 데이터 인덱스

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		class UParticleSystem* Particle;

	int32 CurrentSlot = 0;//최근사용한슬롯

protected:
	UPROPERTY(BlueprintReadOnly)
		UCUserWidget_Select* SelectWidget;//선택창

	UPROPERTY(BlueprintReadOnly)
		TArray<class UCUserWidget_SkillSlot*> SlotWidget;//슬롯

public:
	//selectwidget->PlayOnSequence
	FSelectModeOn OnSelectModeOn;
	//selectwidget->PlayOffSequence
	FSelectModeOff OnSelectModeOff;
//function
private:
	UFUNCTION()//actioncomp
		void AbortByTypeChanged(EActionType InPrevType, EActionType InNewType);

	UFUNCTION()
		void OnProjectileBeginOverlap(FHitResult InHitResult);

	void SetSlot(int32 SlotIndex);
	UFUNCTION() void ChangeSlotData(int32 SlotIndex);

protected:
	virtual void BeginPlay() override;


public:
	ACDoAction_HandGun();

	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void DoSkill(int32 InIdx) override;
	virtual void Begin_DoSkill() override;
	virtual void End_DoSkill() override;

	virtual void Reload() override;
	virtual void Begin_Reload() override;
	virtual void End_Reload() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnAim() override;
	virtual void OffAim() override;

	FORCEINLINE class UCAim* GetAim() { return Aim; }

	virtual void OnSelectMode() override;
	virtual void OffSelectMode() override;

	virtual bool CanSelect() override;

};

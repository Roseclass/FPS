#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "CUserWidget_Select.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeSlotData, int32, SlotIndex);

UCLASS()
class FPS_API UCUserWidget_Select : public UUserWidget
{
	GENERATED_BODY()
	
//property
private:
	bool Select = 1;
	int32 Current = 4;
	int32 SkillSlotIndex = 1;
	int32 ButtonDataIndices[4];//0 up 1 down 2 left 3 right

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* Up;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* Down;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* Left;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* Right;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* UpImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* DownImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* LeftImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* RightImage;


	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* Image;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UBackgroundBlur* Blur;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* OnSequence;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* OffSequence;

	UPROPERTY()
		FWidgetAnimationDynamicEvent OnSequenceEvent;

	UPROPERTY()
		FWidgetAnimationDynamicEvent OffSequenceEvent;

	UPROPERTY()
		FWidgetAnimationDynamicEvent OffSelectEvent;


public:
	FChangeSlotData OnChangeSlotData;
//function
private:
	void SetImageDirection(float InDirection);

	UFUNCTION()void SetUpDirection();
	UFUNCTION()void SetDownDirection();
	UFUNCTION()void SetLeftDirection();
	UFUNCTION()void SetRightDirection();
	UFUNCTION()void SetNormalDirection();

protected:
	virtual void NativeConstruct() override;
public:
	void SetData(int32 InDirection, int32 DataIndex, UObject* InResource);
	FORCEINLINE int32 GetData(int32 InDirection) { return ButtonDataIndices[InDirection]; }

	UFUNCTION() void On();
	UFUNCTION() void Off();
	UFUNCTION() void Change();
	UFUNCTION() void PlayOnSequence();
	UFUNCTION() void PlayOffSequence();

	FORCEINLINE bool CanSelect() { return Select; }
	FORCEINLINE int32 GetDataIndex() { return Current==4 ? -1 : ButtonDataIndices[Current]; }

};

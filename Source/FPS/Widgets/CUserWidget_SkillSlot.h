#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "CUserWidget_SkillSlot.generated.h"

UCLASS()
class FPS_API UCUserWidget_SkillSlot : public UUserWidget
{
	GENERATED_BODY()
//property
private:
	float CoolTime;

	bool CanUse = true;

	float CoolDownPlayTime=0.0f;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* Image;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* GlowEffect;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* CoolDown;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* Complete;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* CantUse;

	UPROPERTY()
		FWidgetAnimationDynamicEvent CoolDownEvent;

public:

//function
private:
protected:
	virtual void NativeConstruct() override;

public:
	void SetData(class UObject* InImage, float InTime);
	void PlayCoolDown();
	void PlayComplete();
	void PlayCantUse();
	UFUNCTION()	void CoolDownEnd();
	UFUNCTION()	void Pause();
	UFUNCTION()	void Resume();

	FORCEINLINE bool IsCanUse() { return CanUse; }
};

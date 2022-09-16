#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "CUserWidget_Interact.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractWidgetDelegate);

UCLASS()
class FPS_API UCUserWidget_Interact : public UUserWidget
{
	GENERATED_BODY()
//property
private:
protected:
	UPROPERTY(EditDefaultsOnly)
		float Time=2.0f;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* Circle;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* Success;

	float CurrentTime = 0.0f;

	bool Interacting = 0;
	bool bSuccess = 0;
public:

//function
private:
	UFUNCTION() void SetHidden();
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	//Cure::OnComponentBeginOverlap->player->reducecurse;
	FInteractWidgetDelegate OnInteractWidgetDelegate;


	//cure->OnComponentBeginOverlap;
	UFUNCTION()void StartInteraction();
	UFUNCTION()void CeaseInteraction();
	UFUNCTION()void CompleteInteraction();

	FORCEINLINE bool IsInteracting() { return Interacting; }
	FORCEINLINE bool IsSucceed() { return bSuccess; }
};

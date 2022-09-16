#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "CUserWidget_HealthBar.generated.h"

UCLASS()
class FPS_API UCUserWidget_HealthBar : public UUserWidget
{
	GENERATED_BODY()
//property
private:
protected:

	AActor* OwnerActor;
	class UCStatusComponent* Status;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UProgressBar* Background;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* HealthBarLayer;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* Begin;

public:

//function
private:
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void SetOwner(AActor* InActor);
	UFUNCTION ()void Update();
}; 

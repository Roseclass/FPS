#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_PlayerInfo.generated.h"

UCLASS()
class FPS_API UCUserWidget_PlayerInfo : public UUserWidget
{
	GENERATED_BODY()
	//property
private:
	class UCStatusComponent* Status;
	bool bHealthActive = false;
	bool bManaActive = false;
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* HealthBarShade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* ManaBarShade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* ManaBar;

	float HealthBarShadeRate;
	float ManaBarShadeRate;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* Profile;

public:

	//function
private:
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void Init(float InHealthRate, float InManaRate, UCStatusComponent* InStatus);
	UFUNCTION()void BeginHealth();
	UFUNCTION()void BeginMana();
};

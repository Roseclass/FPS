#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_DamageText.generated.h"

UCLASS()
class FPS_API UCUserWidget_DamageText : public UUserWidget
{
	GENERATED_BODY()
//property
private:
protected:	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DamageText;

public:

//function
private:
protected:
public:
	void SetRedColor(float InRate);
	void SetText(float InDamage);
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Math/Color.h"
#include "CUserWidget_WeaponInfo.generated.h"

UCLASS()
class FPS_API UCUserWidget_WeaponInfo : public UUserWidget
{
	GENERATED_BODY()
//property
private:
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* Image;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* CurCount;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* WholeCount;
public:

//function
private:
protected:
public:
	UFUNCTION(BlueprintCallable)
		void SetWholeText(int32 count);
	UFUNCTION(BlueprintCallable)
		void SetCurText(int32 count);
	UFUNCTION(BlueprintCallable)
		void SetColor(FLinearColor InColor);
};

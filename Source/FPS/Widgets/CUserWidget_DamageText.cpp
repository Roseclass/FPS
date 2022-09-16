#include "Widgets/CUserWidget_DamageText.h"
#include "Global.h"
#include "Components/TextBlock.h"
#include "Styling/SlateColor.h"

void UCUserWidget_DamageText::SetRedColor(float InRate)
{
	if (InRate > 1.0f)
	{
		InRate = (2.0f - InRate) * 0.1f;
		InRate = UKismetMathLibrary::Clamp(InRate, 0.0f, 1.0f);
		InRate *= 255.0f;
		DamageText->SetColorAndOpacity(FColor(255, InRate, InRate, 255));
	}
	else if (InRate < 1.0f)
	{
		InRate *= 255.0f;
		DamageText->SetColorAndOpacity(FColor(InRate, InRate, InRate, 255));
	}
}

void UCUserWidget_DamageText::SetText(float InDamage)
{
	DamageText->SetText(FText::FromString(FString::SanitizeFloat(InDamage)));
}
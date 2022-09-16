#include "Widgets/CUserWidget_WeaponInfo.h"
#include "Global.h"
#include "Math/Color.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UCUserWidget_WeaponInfo::SetWholeText(int32 count)
{
	FString str = "/ "+ FString::FromInt(count);
	FText text=FText::FromString(str);
	WholeCount->SetText(text);
}
void UCUserWidget_WeaponInfo::SetCurText(int32 count)
{
	FString str = FString::FromInt(count);
	FText text = FText::FromString(str);
	CurCount->SetText(text);
	if (!count)SetColor(FLinearColor::Red);
	else SetColor(FLinearColor::White);
}
void UCUserWidget_WeaponInfo::SetColor(FLinearColor InColor)
{
	Image->SetColorAndOpacity(InColor);
	CurCount->SetColorAndOpacity(InColor);
	WholeCount->SetColorAndOpacity(InColor);
}

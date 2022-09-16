#include "Widgets/CUserWidget_Select.h"
#include "Global.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/BackgroundBlur.h"

void UCUserWidget_Select::NativeConstruct()
{
	Up->OnHovered.AddDynamic(this, &UCUserWidget_Select::SetUpDirection);
	Up->OnUnhovered.AddDynamic(this, &UCUserWidget_Select::SetNormalDirection);

	Down->OnHovered.AddDynamic(this, &UCUserWidget_Select::SetDownDirection);
	Down->OnUnhovered.AddDynamic(this, &UCUserWidget_Select::SetNormalDirection);

	Left->OnHovered.AddDynamic(this, &UCUserWidget_Select::SetLeftDirection);
	Left->OnUnhovered.AddDynamic(this, &UCUserWidget_Select::SetNormalDirection);

	Right->OnHovered.AddDynamic(this, &UCUserWidget_Select::SetRightDirection);
	Right->OnUnhovered.AddDynamic(this, &UCUserWidget_Select::SetNormalDirection);

	OnSequenceEvent.BindDynamic(this, &UCUserWidget_Select::On);
	BindToAnimationEvent(OnSequence, OnSequenceEvent, EWidgetAnimationEvent::Started);

	OffSequenceEvent.BindDynamic(this, &UCUserWidget_Select::Off);
	BindToAnimationEvent(OffSequence, OffSequenceEvent, EWidgetAnimationEvent::Finished);

	OffSelectEvent.BindDynamic(this, &UCUserWidget_Select::Change);
	BindToAnimationEvent(OffSequence, OffSelectEvent, EWidgetAnimationEvent::Started);
}

void UCUserWidget_Select::SetData(int32 InDirection, int32 DataIndex, UObject* InResource)
{
	if (InDirection < 0 || InDirection>3)return;
	ButtonDataIndices[InDirection] = DataIndex;
	switch (InDirection)
	{
		case 0:UpImage->Brush.SetResourceObject(InResource);break;
		case 1:DownImage->Brush.SetResourceObject(InResource);break;
		case 2:LeftImage->Brush.SetResourceObject(InResource);break;
		case 3:RightImage->Brush.SetResourceObject(InResource);break;
		default:break;
	}
}

void UCUserWidget_Select::SetImageDirection(float InDirection)
{
	Image->GetDynamicMaterial()->SetScalarParameterValue(FName("Side"), InDirection);
}

void UCUserWidget_Select::SetUpDirection()
{
	SetImageDirection(0.0f);
	Current = 0;
}

void UCUserWidget_Select::SetDownDirection()
{
	SetImageDirection(1.0f);
	Current = 1;
}

void UCUserWidget_Select::SetLeftDirection()
{
	SetImageDirection(2.0f);
	Current = 2;
}

void UCUserWidget_Select::SetRightDirection()
{
	SetImageDirection(3.0f);
	Current = 3;
}

void UCUserWidget_Select::SetNormalDirection()
{
	SetImageDirection(4.0f);
	Current = 4;
}

void UCUserWidget_Select::On()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
	Select = 0;
}

void UCUserWidget_Select::Off()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	Select = 1;
}

void UCUserWidget_Select::Change()
{
	if (OnChangeSlotData.IsBound())
		OnChangeSlotData.Broadcast(SkillSlotIndex);
}

void UCUserWidget_Select::PlayOnSequence()
{
	PlayAnimationForward(OnSequence, 1.0f);
}

void UCUserWidget_Select::PlayOffSequence()
{
	PlayAnimationForward(OffSequence, 1.0f);
}
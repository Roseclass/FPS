#include "Widgets/CUserWidget_SkillSlot.h"
#include "Global.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"

void UCUserWidget_SkillSlot::NativeConstruct()
{
	CoolDownEvent.BindDynamic(this, &UCUserWidget_SkillSlot::CoolDownEnd);
	BindToAnimationEvent(CoolDown, CoolDownEvent, EWidgetAnimationEvent::Finished);
}

void UCUserWidget_SkillSlot::SetData(UObject* InImage, float InTime)
{
	if (InImage)	Image->Brush.SetResourceObject(InImage);
	else Image->RenderOpacity = 0.0f;
	CoolTime = InTime;		
}

void UCUserWidget_SkillSlot::PlayCoolDown()
{
	CanUse = false;
	PlayAnimationForward(CoolDown, 1.0f / CoolTime);
}

void UCUserWidget_SkillSlot::PlayComplete()
{
	PlayAnimationForward(Complete, 1.0f);
}

void UCUserWidget_SkillSlot::PlayCantUse()
{
	PlayAnimationForward(CantUse, 1.0f);
}

void UCUserWidget_SkillSlot::CoolDownEnd()
{
	CanUse = true;
	PlayComplete();
}

void UCUserWidget_SkillSlot::Pause()
{
	if (!IsAnimationPlaying(CoolDown))
	{
		CoolDownPlayTime=-1.0f;
		return;
	}
	CoolDownPlayTime = PauseAnimation(CoolDown);
}

void UCUserWidget_SkillSlot::Resume()
{
	if (CoolDownPlayTime == -1.0f)return;
	PlayAnimation(CoolDown, CoolDownPlayTime, 1, EUMGSequencePlayMode::Forward, 1.0f / CoolTime);
}

#include "Widgets/CUserWidget_Interact.h"
#include "Global.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

void UCUserWidget_Interact::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!Interacting)return;
	if (bSuccess)return;
	CurrentTime += InDeltaTime / Time;
	Circle->GetDynamicMaterial()->SetScalarParameterValue("Percent", CurrentTime);
	if (CurrentTime >= 1.0f) CompleteInteraction();
}

void UCUserWidget_Interact::StartInteraction()
{
	Interacting = 1;
}

void UCUserWidget_Interact::CeaseInteraction()
{
	Interacting = 0;
	CurrentTime = 0.0f;
	Circle->GetDynamicMaterial()->SetScalarParameterValue("Percent", CurrentTime);
}

void UCUserWidget_Interact::CompleteInteraction()
{
	FWidgetAnimationDynamicEvent f;
	f.BindDynamic(this, &UCUserWidget_Interact::SetHidden);
	BindToAnimationFinished(Success, f);
	PlayAnimationForward(Success);
	bSuccess = 1;
	if (OnInteractWidgetDelegate.IsBound())
		OnInteractWidgetDelegate.Broadcast();
}

void UCUserWidget_Interact::SetHidden()
{
	SetVisibility(ESlateVisibility::Hidden);
}

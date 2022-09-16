#include "Widgets/CUserWidget_HealthBar.h"
#include "Global.h"
#include "Components/ProgressBar.h"
#include "Components/CStatusComponent.h"

void UCUserWidget_HealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimationForward(Begin);
}

void UCUserWidget_HealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CheckNull(Status);
	HealthBarLayer->SetPercent(UKismetMathLibrary::FInterpTo(HealthBarLayer->Percent,Status->GetHealthRatio(),InDeltaTime,1.0f));
}

void UCUserWidget_HealthBar::SetOwner(AActor* InActor)
{
	OwnerActor = InActor;

	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerActor);
	Update();
}

void UCUserWidget_HealthBar::Update()
{
	CheckNull(Status);
	HealthBar->SetPercent(Status->GetHealthRatio());
}

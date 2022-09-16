#include "Widgets/CUserWidget_PlayerInfo.h"
#include "Global.h"
#include "Components/ProgressBar.h"
#include "Components/CStatusComponent.h"

void UCUserWidget_PlayerInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCUserWidget_PlayerInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bHealthActive)
	{
		float temp = UKismetMathLibrary::FInterpTo(HealthBarShadeRate, HealthBar->Percent, InDeltaTime, 1.0f);
		HealthBarShade->SetPercent(temp);
		HealthBarShadeRate = temp;
	}

	if (bManaActive)
	{
		float temp = UKismetMathLibrary::FInterpTo(ManaBarShadeRate, ManaBar->Percent, InDeltaTime, 1.0f);
		ManaBarShade->SetPercent(temp);
		ManaBarShadeRate = temp;
	}
}

void UCUserWidget_PlayerInfo::Init(float InHealthRate, float InManaRate, UCStatusComponent* InStatus)
{
	Status = InStatus;
	Status->OnAddHealth.AddDynamic(this, &UCUserWidget_PlayerInfo::BeginHealth);
	Status->OnSubHealth.AddDynamic(this, &UCUserWidget_PlayerInfo::BeginHealth);

	HealthBar->SetPercent(InHealthRate);
	HealthBarShade->SetPercent(InHealthRate);
	HealthBarShadeRate = InHealthRate;

	ManaBar->SetPercent(InManaRate);
	ManaBarShade->SetPercent(InManaRate);
	ManaBarShadeRate = InManaRate;

}

void UCUserWidget_PlayerInfo::BeginHealth()
{
	float InRate = Status->GetHealthRatio();
	if (InRate >= HealthBarShadeRate)
	{
		bHealthActive = false;
		HealthBar->SetPercent(InRate);
		HealthBarShade->SetPercent(InRate);
		HealthBarShadeRate = InRate;
	}
	else
	{
		bHealthActive = true;
		HealthBar->SetPercent(InRate);
	}
}

void UCUserWidget_PlayerInfo::BeginMana()
{
	float InRate = 1.0f;
	if (InRate >= ManaBarShadeRate)
	{
		bManaActive = false;
		ManaBar->SetPercent(InRate);
		ManaBarShade->SetPercent(InRate);
		ManaBarShadeRate = InRate;
	}
	else
	{
		bManaActive = true;
		ManaBar->SetPercent(InRate);
	}
}

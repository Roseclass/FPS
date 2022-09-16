#include "Components/CStatusComponent.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/CUserWidget_PlayerInfo.h"
#include "GameFramework/Character.h"

UCStatusComponent::UCStatusComponent()
{

}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	if (PlayerInfoClass)
	{
		PlayerInfo = CreateWidget<UCUserWidget_PlayerInfo, APlayerController>(Cast<ACharacter>(GetOwner())->GetController<APlayerController>(), PlayerInfoClass);
		PlayerInfo->AddToViewport();
		PlayerInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
		PlayerInfo->Init(GetHealthRatio(), 1.0f,this);
	}
}

void UCStatusComponent::SetMove()
{
	bCanMove = true;
}

void UCStatusComponent::SetStop()
{
	bCanMove = false;
}

void UCStatusComponent::SetSpeed(ECharacterSpeed InSpeed)
{
	UCharacterMovementComponent* movement = CHelpers::GetComponent<UCharacterMovementComponent>(GetOwner());

	movement->MaxWalkSpeed = Speed[(int32)InSpeed];
}

void UCStatusComponent::AddHealth(float InAmount)
{
	Health += InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	if (OnAddHealth.IsBound())
		OnAddHealth.Broadcast();
}

void UCStatusComponent::SubHealth(float InAmount)
{
	Health -= InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	if (OnSubHealth.IsBound())
		OnSubHealth.Broadcast();

}
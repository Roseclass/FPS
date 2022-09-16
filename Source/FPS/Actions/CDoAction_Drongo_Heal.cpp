#include "Actions/CDoAction_Drongo_Heal.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CBehaviorComponent.h"
#include "Characters/CEnemy.h"
ACDoAction_Drongo_Heal::ACDoAction_Drongo_Heal()
{

}

void ACDoAction_Drongo_Heal::BeginPlay()
{
	Super::BeginPlay();

	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
}

void ACDoAction_Drongo_Heal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bHealing) Status->AddHealth(DeltaTime * 5.0f);
}

void ACDoAction_Drongo_Heal::DoAction()
{
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();

	bHealing = 1;

}

void ACDoAction_Drongo_Heal::Begin_DoAction()
{

}

void ACDoAction_Drongo_Heal::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
	bHealing = 0;
	Cast<ACEnemy>(OwnerCharacter)->Hide_End();
}

void ACDoAction_Drongo_Heal::Abort()
{
	OwnerCharacter->StopAnimMontage();
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(OwnerCharacter->GetController());
	behavior->SetWaitMode();
	End_DoAction();
}

#include "Actions/CDoAction_Drongo_Hide.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Characters/CEnemy.h"

ACDoAction_Drongo_Hide::ACDoAction_Drongo_Hide()
{

}

void ACDoAction_Drongo_Hide::BeginPlay()
{
	Super::BeginPlay();
}

void ACDoAction_Drongo_Hide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDoAction_Drongo_Hide::DoAction()
{
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Drongo_Hide::Begin_DoAction()
{

}

void ACDoAction_Drongo_Hide::End_DoAction()
{
	Cast<ACEnemy>(OwnerCharacter)->Hide();
	State->SetIdleMode();
	Status->SetMove();
}

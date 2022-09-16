#include "BehaviorTree/CBTTaskNode_Drongo_Change.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy.h" 
#include "Components/CStateComponent.h" 

UCBTTaskNode_Drongo_Change::UCBTTaskNode_Drongo_Change()
{
	NodeName = "Change";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Drongo_Change::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* aiPawn = Cast<ACEnemy>(controller->GetPawn());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(aiPawn);

	if (action->IsTwoHandMode())
	{
		action->SetHandGunMode();
	}
	else if (action->IsHandGunMode())
	{
		action->SetOneHandMode();
	}
	else
	{
		action->SetTwoHandMode();
	}


	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Drongo_Change::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* aiPawn = Cast<ACEnemy>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	if (state->IsIdleMode())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}


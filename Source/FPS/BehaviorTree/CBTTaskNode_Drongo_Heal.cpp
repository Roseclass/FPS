#include "BehaviorTree/CBTTaskNode_Drongo_Heal.h"
#include "Global.h"
#include "Characters/CEnemy.h"
#include "Characters/CAIController.h"
#include "Components/CActionComponent.h"
#include "Components/CBehaviorComponent.h"

UCBTTaskNode_Drongo_Heal::UCBTTaskNode_Drongo_Heal()
{
	NodeName = "Heal";

	bNotifyTick = true;

}

EBTNodeResult::Type UCBTTaskNode_Drongo_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	TotalTime = 0.0f;
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* aiPawn = Cast<ACEnemy>(controller->GetPawn());
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(aiPawn);
	action->SetUnarmedMode();
	action->SetWarpMode();
	action->DoAction();
	Delay = aiPawn->GetCurrentMontage()->GetPlayLength() + 0.5f;

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Drongo_Heal::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	TotalTime += DeltaSeconds;
	if (TotalTime > Delay)
	{
		UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(OwnerComp.GetOwner());
		behavior->SetWaitMode();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

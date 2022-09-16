#include "BehaviorTree/CBTTaskNode_Drongo_Hide.h"
#include "Global.h"
#include "Characters/CEnemy.h"
#include "Characters/CAIController.h"
#include "Components/CActionComponent.h"
#include "Components/CBehaviorComponent.h"

UCBTTaskNode_Drongo_Hide::UCBTTaskNode_Drongo_Hide()
{
	NodeName = "Hide";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Drongo_Hide::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	TotalTime = 0.0f;
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* aiPawn = Cast<ACEnemy>(controller->GetPawn());
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(aiPawn);
	action->SetUnarmedMode();
	action->SetTornadoMode();
	action->DoAction();
	Delay = aiPawn->GetCurrentMontage()->GetPlayLength() + 1.5f;

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Drongo_Hide::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	TotalTime += DeltaSeconds;
	if (TotalTime > Delay)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


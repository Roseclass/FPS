#include "BehaviorTree/CBTTaskNode_Emotion.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Emotion::UCBTTaskNode_Emotion()
{
	NodeName = "Emotion";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Emotion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* aiPawn = Cast<ACEnemy>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	state->SetEmotionMode();
	Delay = aiPawn->GetCurrentMontage()->GetPlayLength() + 0.5f;
	TotalTime = 0.0f;
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Emotion::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* aiPawn = Cast<ACEnemy>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	TotalTime += DeltaSeconds;

	if (/*state->IsIdleMode() &&*/ TotalTime > Delay)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

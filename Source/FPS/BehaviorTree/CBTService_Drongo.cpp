#include "BehaviorTree/CBTService_Drongo.h"
#include "Global.h"
#include "Characters/CEnemy.h"
#include "Characters/CPlayer.h"
#include "Characters/CAIController.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CBehaviorComponent.h"

UCBTService_Drongo::UCBTService_Drongo()
{
	NodeName = "Drongo";
}

void UCBTService_Drongo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckTrue(behavior->IsAvoidMode());

	ACEnemy* aiPawn = Cast<ACEnemy>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(aiPawn);

	if (state->IsDeadMode())
	{
		behavior->SetWaitMode();
		return;
	}
	if (status->GetHealthRatio() < (AvoidCount + 1) * AvoidRatio)
	{
		AvoidCount--;
		behavior->IncreaseActCount(false);
		behavior->SetAvoidMode();
		return;
	}
	behavior->SetApproachMode();
	//ACPlayer* target = behavior->GetTargetPlayer();
	//if (target == nullptr)
	//{
	//	behavior->SetWaitMode();
	//	return;
	//}
	//else
	//{
	//	UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
	//	if (targetState->IsDeadMode())
	//	{
	//		behavior->SetWaitMode();
	//		return;
	//	}
	//}
	//
	//float distance = aiPawn->GetDistanceTo(target);
	//
	//if (distance < controller->GetBehaviorRange())
	//{
	//	behavior->SetActionMode();
	//	return;
	//}
	//
	//if (distance < controller->GetSightRadius())
	//{
	//	behavior->SetApproachMode();
	//	return;
	//}
}

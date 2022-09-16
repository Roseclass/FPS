#include "BehaviorTree/CBTTaskNode_MoveToLocation.h"
#include "Global.h"
#include "AIController.h"
#include "Characters/CAIController.h"
#include "Characters/CPlayer.h" 
#include "Characters/CAnimInstance.h" 
#include "Components/CStatusComponent.h" 
#include "Components/CBehaviorComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/AIAsyncTaskBlueprintProxy.h"

UCBTTaskNode_MoveToLocation::UCBTTaskNode_MoveToLocation()
{
	NodeName = "MoveStart";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_MoveToLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	Speed = 0.0f;
	Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	Status = CHelpers::GetComponent<UCStatusComponent>(Controller->GetPawn());
	Movement = CHelpers::GetComponent<UCharacterMovementComponent>(Controller->GetPawn());
	Behavior = CHelpers::GetComponent<UCBehaviorComponent>(Controller);

	UAIAsyncTaskBlueprintProxy* f = UAIBlueprintHelperLibrary::CreateMoveToProxyObject(Controller->GetWorld(), Controller->GetPawn(), Behavior->GetWarpLocation(),nullptr, AcceptanceRadius);
	f->OnSuccess.AddDynamic(this,&UCBTTaskNode_MoveToLocation::OnSuccess);
	f->OnFail.AddDynamic(this, &UCBTTaskNode_MoveToLocation::OnFail);

	Anim = Cast<UCAnimInstance>(Controller->GetCharacter()->GetMesh()->GetAnimInstance());
	Anim->SetAIAccelerating(1);

	flag = 0;

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_MoveToLocation::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Stop();
	CLog::Print("Abort");
	return EBTNodeResult::Aborted;
}

void UCBTTaskNode_MoveToLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	Speed += DeltaSeconds * Status->GetRunSpeed();
	Speed = UKismetMathLibrary::FClamp(Speed, 0.0f, Status->GetRunSpeed());
	Movement->MaxWalkSpeed = Speed;

	Controller = Cast<ACAIController>(OwnerComp.GetOwner());

	if(flag==1)
	{
		CLog::Print("Succeeded");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else if (flag == 2)
	{
		CLog::Print("Fail");
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

void UCBTTaskNode_MoveToLocation::Stop()
{
	Anim->SetAIAccelerating(0);
	Movement->MaxWalkSpeed = 0.0f;
}

void UCBTTaskNode_MoveToLocation::OnSuccess(EPathFollowingResult::Type MovementResult)
{
	Stop();
	flag = 1;
}

void UCBTTaskNode_MoveToLocation::OnFail(EPathFollowingResult::Type MovementResult)
{
	Stop();
	flag = 2;
}

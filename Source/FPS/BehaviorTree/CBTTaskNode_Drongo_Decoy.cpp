#include "BehaviorTree/CBTTaskNode_Drongo_Decoy.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy.h" 
#include "Characters/CEnemy_Decoy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UCBTTaskNode_Drongo_Decoy::UCBTTaskNode_Drongo_Decoy()
{
	CHelpers::GetClass(&DecoyClass, "Blueprint'/Game/Enemies/Drongo/Decoy/BP_CEnemy_Decoy_Drongo.BP_CEnemy_Decoy_Drongo_C'");
}

EBTNodeResult::Type UCBTTaskNode_Drongo_Decoy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* aiPawn = Cast<ACEnemy>(controller->GetPawn());

	FVector start = aiPawn->GetActorLocation(),
	end = aiPawn->GetActorForwardVector()*50.0f;
	SpawnDecoy(start + (end) , start + (end * 20));
	SpawnDecoy(start - (end) , start - (end * 20));
	end = aiPawn->GetActorRightVector()*50.0f;
	SpawnDecoy(start + (end), start + (end * 20));
	SpawnDecoy(start - (end), start - (end * 20));
	return EBTNodeResult::Succeeded;
}

void UCBTTaskNode_Drongo_Decoy::SpawnDecoy(FVector InStart, FVector InEnd)
{
	FTransform transform;
	transform.SetLocation(InStart);
	transform.SetRotation(FQuat(UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::Normal(InEnd - InStart))));
	ACEnemy_Decoy* decoy = Cast<ACEnemy_Decoy>(GetWorld()->SpawnActor(DecoyClass, &transform));
	UAIBlueprintHelperLibrary::CreateMoveToProxyObject(decoy->GetWorld(), decoy, InEnd);
	decoy->SetLifeSpan(15.0f);
}

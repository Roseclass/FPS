#include "BehaviorTree/CBTTaskNode_Curse.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Objects/CInteract_Cure.h"


UCBTTaskNode_Curse::UCBTTaskNode_Curse()
{
	NodeName = "Curse";
}

EBTNodeResult::Type UCBTTaskNode_Curse::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	TArray<AActor*> arr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPlayer::StaticClass(), arr);
	if (arr.Num())
	{
		Cast<ACPlayer>(arr[0])->StartCurse();
	}
	TArray<AActor*> cure;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACInteract_Cure::StaticClass(), cure);
	for (auto i : cure)
		Cast<ACInteract_Cure>(i)->SetActive();

	return EBTNodeResult::Succeeded;
}

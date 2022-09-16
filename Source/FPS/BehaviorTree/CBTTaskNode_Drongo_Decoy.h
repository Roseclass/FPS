#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Drongo_Decoy.generated.h"

UCLASS()
class FPS_API UCBTTaskNode_Drongo_Decoy : public UBTTaskNode
{
	GENERATED_BODY()
//property
private:
	TSubclassOf<class ACEnemy_Decoy> DecoyClass;
protected:
public:

//function
private:
	void SpawnDecoy(FVector InStart, FVector InEnd);
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	UCBTTaskNode_Drongo_Decoy();
};

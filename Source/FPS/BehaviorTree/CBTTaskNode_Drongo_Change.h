#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CActionComponent.h"
#include "CBTTaskNode_Drongo_Change.generated.h"

UCLASS()
class FPS_API UCBTTaskNode_Drongo_Change : public UBTTaskNode
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(EditAnywhere)
		EActionType Type;

protected:
public:

//function
private:
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UCBTTaskNode_Drongo_Change();

};

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Action.generated.h"

UCLASS()
class FPS_API UCBTTaskNode_Action : public UBTTaskNode
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(EditAnywhere, Category = "AI")
		float Delay = 0.5f;

	float TotalTime = 0.0f;
	class UCStateComponent* State;
protected:
public:

//function
private:
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UCBTTaskNode_Action();

};

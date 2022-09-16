#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "CBTTaskNode_MoveToLocation.generated.h"

UCLASS()
class FPS_API UCBTTaskNode_MoveToLocation : public UBTTaskNode
{
	GENERATED_BODY()
//property
private:
	float Speed;

	int32 flag;

	class ACAIController* Controller;
	class UCStatusComponent* Status;
	class UCAnimInstance* Anim;
	class UCharacterMovementComponent* Movement;
	class UCBehaviorComponent* Behavior;
protected:
	UPROPERTY(EditDefaultsOnly)
		float AcceptanceRadius = 20.0f;
public:

//function
private:
	void Stop();
	UFUNCTION()void OnSuccess(EPathFollowingResult::Type MovementResult);
	UFUNCTION()void OnFail(EPathFollowingResult::Type MovementResult);
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
public:
	UCBTTaskNode_MoveToLocation();

};

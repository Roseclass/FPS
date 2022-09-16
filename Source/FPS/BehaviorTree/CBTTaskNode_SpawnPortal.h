#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_SpawnPortal.generated.h"

UCLASS()
class FPS_API UCBTTaskNode_SpawnPortal : public UBTTaskNode
{
	GENERATED_BODY()
	//property
private:
	float Speed;

	class UCStatusComponent* Status;
	class UCAnimInstance* Anim;
	class UCharacterMovementComponent* Movement;

	TSubclassOf<class ACPortal> PortalClass;
	class ACPortal* A;
	ACPortal* B;

	bool Flag = 0;
protected:
public:

	//function
private:
	UFUNCTION() 
		void PortalEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION() 
		void Test1(EPathFollowingResult::Type MovementResult);
	UFUNCTION() 
		void Test2(EPathFollowingResult::Type MovementResult);
protected:
public:
	UCBTTaskNode_SpawnPortal();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};

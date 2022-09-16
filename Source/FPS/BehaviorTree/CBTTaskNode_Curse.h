#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Curse.generated.h"

UCLASS()
class FPS_API UCBTTaskNode_Curse : public UBTTaskNode
{
	GENERATED_BODY()
//property
private:
	TSubclassOf<class ACProjectile> ProjectileClass;
	class ACProjectile_AirStrike* Projectile;

	class UCStateComponent* State;
protected:
public:

//function
private:
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UCBTTaskNode_Curse();

};

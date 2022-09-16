#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Drongo.generated.h"

UCLASS()
class FPS_API UCBTService_Drongo : public UBTService
{
	GENERATED_BODY()
//property
private:
	float AvoidRatio = 0.3f;
	int32 AvoidCount = 2;
protected:
public:

//function
private:
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UCBTService_Drongo();

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Drongo_Hide.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UCBTTaskNode_Drongo_Hide : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "AI")
		float Delay = 2.0f;

	float TotalTime = 0.0f;
protected:
public:

	//function
private:
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UCBTTaskNode_Drongo_Hide();

};

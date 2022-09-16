#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class FPS_API ACAIController : public AAIController
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(EditAnywhere)
		float BehaviorRange = 150.0f;

	UPROPERTY(EditAnywhere)
		bool bDrawDebug = 0;

	UPROPERTY(EditAnywhere)
		float AdjustCircleHeight = 50.0f;

	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

	class ACEnemy* OwnerEnemy;

	class UAISenseConfig_Sight* Sight;

protected:
public:

//function
private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	ACAIController();

	FORCEINLINE float GetBehaviorRange() { return BehaviorRange; }
	float GetSightRadius();

};

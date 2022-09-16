#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Wait, Approach, Action, Avoid, Special, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChanged, EBehaviorType, InPrevType, EBehaviorType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UCBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

	//property
private:
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "Behavior";

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "Player";

	UPROPERTY(EditAnywhere)
		FName WarpLocation = "WarpLocation";

	UPROPERTY(EditAnywhere)
		FName ActCount = "Act";

	UPROPERTY(EditAnywhere)
		int32 MaxActCount;

	FRotator ControlRot;
	class ACAIController* Controller;
	class UBlackboardComponent* Blackboard;
	class ACharacter* Character;
	class UCAnimInstance* Anim;

protected:
public:

	//function
private:
	void ChangeType(EBehaviorType InNewType);
	void CalculateYawPitch();
	EBehaviorType GetType();

protected:
	virtual void BeginPlay() override;

public:		
	UCBehaviorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Init();

	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetAvoidMode();
	void SetSpecialMode();

	class ACPlayer* GetTargetPlayer();

	FVector GetWarpLocation();

	UFUNCTION(BlueprintPure)
		bool IsWaitMode();

	UFUNCTION(BlueprintPure)
		bool IsApproachMode();

	UFUNCTION(BlueprintPure)
		bool IsActionMode();

	UFUNCTION(BlueprintPure)
		bool IsAvoidMode();

	UFUNCTION(BlueprintPure)
		bool IsSpecialMode();

	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

	UPROPERTY(BlueprintAssignable)
		FBehaviorTypeChanged OnBehaviorTypeChanged;

	void IncreaseActCount(bool IsRepeat = true);
};

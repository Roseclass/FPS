#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CActionComponent.h"
#include "Components/CFeetComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class FPS_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
//property
private:
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool AI;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool IsAccelerating;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool IsInAir;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EActionType ActionType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Yaw;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Pitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Roll;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator PrevRot;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float YawDelta;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool IsUnarmed;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "YawDeltaOption")
		float LeanIntensity = 7.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "YawDeltaOption")
		float InterpSpeed = 6.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FFeetData FeetData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bIkMode;

public:

//function
private:
	UFUNCTION()
		void OnActionTypeChaged(EActionType InPrevType, EActionType InNewType);

protected:
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetAIAccelerating(bool Inbool);
	void SetAIUnarmedState(bool Inbool);
	void SetAIYaw(float InYaw);
	void SetAIPitch(float InPitch);

};

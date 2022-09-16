
#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Drongo_Gun.generated.h"

UCLASS()
class FPS_API ACDoAction_Drongo_Gun : public ACDoAction
{
	GENERATED_BODY()
//property
private:
	bool LockOn = 1;
	FRotator ControlRot;
	class UCAnimInstance* Anim;
	class UCBehaviorComponent* Behavior;
	class ACAIController* Controller;
	TSubclassOf<class UUserWidget> TargetingWidgetClass;
	FTimerHandle ActionTimerHandle;
	UUserWidget* ActionWidget;
protected:
public:

//function
private:
	void StartLockOn();
	void EndLockOn();

	UFUNCTION()
		void OnProjectileBeginOverlap(FHitResult InHitResult);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	ACDoAction_Drongo_Gun();

	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;
	virtual void Abort() override;

};

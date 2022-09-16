#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Drongo_Throw.generated.h"

UCLASS()
class FPS_API ACDoAction_Drongo_Throw : public ACDoAction
{
	GENERATED_BODY()
//property
private:
	TSubclassOf<class ACProjectile_Path> PathClass;
protected:
public:

//function
private:
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	ACDoAction_Drongo_Throw();

	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;


};

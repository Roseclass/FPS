#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Drongo_Hide.generated.h"

UCLASS()
class FPS_API ACDoAction_Drongo_Hide : public ACDoAction
{
	GENERATED_BODY()
//property
private:
protected:
public:

//function
private:
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	ACDoAction_Drongo_Hide();

	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;
};

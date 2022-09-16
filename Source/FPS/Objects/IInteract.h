#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteract.generated.h"

UINTERFACE(MinimalAPI)
class UIInteract : public UInterface
{
	GENERATED_BODY()
};

class FPS_API IIInteract
{
	GENERATED_BODY()
public:
	virtual void RevealWidget() {};
	virtual void ConcealWidget() {};
	virtual void Interact() {};
	virtual void Interact_End() {};
	virtual void InteractCease() {};
	virtual void InteractComplete() {};

};

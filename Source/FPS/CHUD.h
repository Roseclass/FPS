#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

UCLASS()
class FPS_API ACHUD : public AHUD
{
	GENERATED_BODY()
//property
private:
	bool bDraw;

	UPROPERTY(EditDefaultsOnly)
		class UTexture2D* Texture;

protected:
public:

//function
private:
protected:
public:
	ACHUD();

	virtual void DrawHUD() override;

	FORCEINLINE void SetDraw() { bDraw = true; }
	FORCEINLINE void SetNoDraw() { bDraw = false; }

};

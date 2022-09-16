#pragma once

#include "CoreMinimal.h"
#include "Actions/CProjectile.h"
#include "CProjectile_Obliterate.generated.h"

UCLASS()
class FPS_API ACProjectile_Obliterate : public ACProjectile
{
	GENERATED_BODY()

//property
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACProjectile> ChildClass;
protected:
public:

//function
private:

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SpawnChild() override;

public:
	ACProjectile_Obliterate();

};

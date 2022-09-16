#pragma once

#include "CoreMinimal.h"
#include "Actions/CProjectile.h"
#include "CProjectile_AirStrike.generated.h"

UCLASS()
class FPS_API ACProjectile_AirStrike : public ACProjectile
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACProjectile> ChildClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> DecalClass;

	UPROPERTY(EditAnywhere)
		int32 Count=10;
	int32 SpawnCount = 0;
	float time;
	UPROPERTY(EditAnywhere)
		float Term = 1.0f;

	bool Finish=0;
protected:
public:

//function
private:
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


public:
	ACProjectile_AirStrike();

	bool IsFinish() { return Finish; }
	virtual void SpawnChild() override;
};

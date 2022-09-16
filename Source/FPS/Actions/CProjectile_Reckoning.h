#pragma once

#include "CoreMinimal.h"
#include "Actions/CProjectile.h"
#include "CProjectile_Reckoning.generated.h"

UCLASS()
class FPS_API ACProjectile_Reckoning : public ACProjectile
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
		float TickTime = 0.3;
	float Time = 0.0f;
	TMap<class ACEnemy*, bool> Enemies;
protected:
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

public:
	ACProjectile_Reckoning();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};

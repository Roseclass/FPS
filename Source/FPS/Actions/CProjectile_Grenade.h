#pragma once

#include "CoreMinimal.h"
#include "Actions/CProjectile.h"
#include "Components/TimelineComponent.h"
#include "CProjectile_Grenade.generated.h"

UCLASS()
class FPS_API ACProjectile_Grenade : public ACProjectile
{
	GENERATED_BODY()
private:
	TMap<AActor*, bool> Map;
	bool bActive = 0;
	FTimeline OverlapTimeline;
	FOnTimelineFloat OverlapFloat;
	UPROPERTY(EditDefaultsOnly)
		class UCurveFloat* OverlapCurve;
	UPROPERTY(EditDefaultsOnly)
		float CountTime=5.0f;

	UFUNCTION() void EndFunction();
protected:
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
public:
	ACProjectile_Grenade();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};

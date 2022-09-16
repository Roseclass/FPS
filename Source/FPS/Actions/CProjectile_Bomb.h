#pragma once

#include "CoreMinimal.h"
#include "Actions/CProjectile.h"
#include "Components/TimelineComponent.h"
#include "CProjectile_Bomb.generated.h"

UCLASS()
class FPS_API ACProjectile_Bomb : public ACProjectile
{
	GENERATED_BODY()
private:
	TMap<AActor*, bool> Map;
	bool bActive = 0;
	FTimeline OverlapTimeline;
	FOnTimelineFloat OverlapFloat;
	UPROPERTY(EditDefaultsOnly)
		class UCurveFloat* OverlapCurve;
	UFUNCTION() void EndFunction();
protected:
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
public:
	ACProjectile_Bomb();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};

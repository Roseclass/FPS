#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TimelineComponent.h"
#include "CAim.generated.h"

UCLASS()
class FPS_API UCAim : public UObject
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(EditAnywhere)
		class UCurveFloat* Curve;

	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

	bool bInZoom;

	FTimeline Timeline;
	FOnTimelineFloat TimelineFloat;

	class ACHUD* Hud;

protected:
public:

//function
private:
	UFUNCTION()
		void Zooming(float Value);

protected:
public:
	UCAim();

	void BeginPlay(class ACharacter* InCharacter);
	void Tick(float DeltaTime);

	void On();
	void Off();

	FORCEINLINE bool IsAvaliable() { return SpringArm != nullptr && Camera != nullptr; }
	FORCEINLINE bool InZoom() { return bInZoom; }

};

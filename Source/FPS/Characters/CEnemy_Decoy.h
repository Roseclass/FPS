#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Components/TimelineComponent.h"
#include "CEnemy_Decoy.generated.h"

UCLASS()
class FPS_API ACEnemy_Decoy : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	//property
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1;

	FTimeline PiercingTimeline;
	FOnTimelineFloat PiercingFloat;
	UPROPERTY(EditDefaultsOnly, Category = "Piercing")
		UCurveFloat* PiercingCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Piercing")
		class UMaterialParameterCollection* PiercingMPC;

	UPROPERTY(EditDefaultsOnly, Category = "Piercing")
		FName PiercingRadiusName;

	UPROPERTY(EditDefaultsOnly, Category = "Piercing")
		FName PiercingLocationName;
protected:
public:

	//function
private:
protected:
	virtual void BeginPlay() override;
	UFUNCTION()void Piercing(float Value);
public:
	ACEnemy_Decoy();
	virtual void Tick(float DeltaTime) override;	
	void DoPierce(FVector InLocation);
};

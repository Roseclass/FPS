#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPortalComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UCPortalComponent : public UActorComponent
{
	GENERATED_BODY()
//property
private:
	float MaxSpawnDistance= 10000.0f;
	class ACPortal* PortalA;
	ACPortal* PortalB;

	UPROPERTY(EditDefaultsOnly, Category = "LineTrace")
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "LineTrace")
		TArray<AActor*> Actors;

protected:
public:

//function
private:
	void SwapPortals(ACPortal*& Portal, ACPortal* NewPortal);
protected:
	virtual void BeginPlay() override;
public:
	UCPortalComponent();

	UFUNCTION(BlueprintCallable)
		void SpawnPortalAlongVector(FVector StartLocation, FVector Direction, bool IsPortalA);
};

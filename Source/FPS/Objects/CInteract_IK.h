#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CInteract_IK.generated.h"

UCLASS()
class FPS_API ACInteract_IK : public AActor
{
	GENERATED_BODY()
	//property
private:
	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* Box;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* Mesh;

protected:
public:

	//function
private:
protected:
	virtual void BeginPlay() override;
public:
	ACInteract_IK();
	virtual void Tick(float DeltaTime) override;

};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteract.h"
#include "CInteract_Wall.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartWallDelegate);

UCLASS()
class FPS_API ACInteract_Wall : public AActor, public IIInteract
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
	//player->setinteraction,interact->startinteraction;
	FStartWallDelegate OnStartDelegate;

//function
private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

public:
	ACInteract_Wall();

	virtual void Interact() override;
};

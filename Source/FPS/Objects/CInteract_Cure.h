#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteract.h"
#include "CInteract_Cure.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCeaseDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCompleteDelegate);

UCLASS()
class FPS_API ACInteract_Cure : public AActor ,public IIInteract
{
	GENERATED_BODY()
	
//property
private:
	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* Box;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
		class UWidgetComponent* Widget;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* Effect;

	bool bActive = 0;
	
protected:
public:
	//player->setinteraction,interact->startinteraction;
	FStartDelegate OnStartDelegate;
	//interact->ceaseinteraction
	FCeaseDelegate OnCeaseDelegate;
	//player->reducecurse,interact->endinteraction;
	FCompleteDelegate OnCompleteDelegate;

//function
private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void PlayParticleEffect();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


public:
	ACInteract_Cure();

	//UIInteract
	virtual void RevealWidget() override;
	virtual void ConcealWidget() override;
	virtual void Interact() override;
	virtual void Interact_End() override;
	virtual void InteractCease() override;
	UFUNCTION()virtual void InteractComplete() override;

	void SetActive();
};

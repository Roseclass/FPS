#pragma once

#include "CoreMinimal.h"
#include "NinjaCharacter.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "ICharacter.h"
#include "GenericTeamAgentInterface.h"
#include "CNPlayer.generated.h"

UCLASS()
class FPS_API ACNPlayer : public ANinjaCharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
		//property
private:
	UPROPERTY(EditDefaultsOnly, Category = "TeamID")
		uint8 TeamID = 0;

	//UPROPERTY(EditDefaultsOnly, Category = "Smear")
	//	float SmearLength = 0.1f;

	//UPROPERTY(EditDefaultsOnly)
	//	class UCDataAsset* Test_DataAsset;

	//UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	//	TSubclassOf<class UCUserWidget_Select> SelectWidgetClass;

	//SceneComp
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	//ActorComp
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

	//UPROPERTY(VisibleDefaultsOnly)
	//	class UCFeetComponent* Feet;

	//UPROPERTY(VisibleDefaultsOnly)
	//	class UPostProcessComponent* PostProcess;

	class AController* DamageInstigator;
	//class ACInteractDoor* InteractDoor;

protected:
	//UPROPERTY(BlueprintReadOnly)
	//	class UCUserWidget_Select* SelectWidget;
public:

	//function
private:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);
	void OnZoom(float InAxis);
	void OnWalk();
	void OffWalk();
	void OnEvade();
	void OnJump();

	UFUNCTION()
		void EndEvade();

	void Begin_BackStep();
	void Begin_Roll();
	void End_BackStep();

	void Hitted();

	UFUNCTION()
		void Hitted_End();

	void Dead();
	void End_Dead();

	UFUNCTION() void OnHandGun();

	void OnDoAction();

	void OnAim();
	void OffAim();

	//void OnSelectAction();
	//void OffSelectAction();

	//void OnInteract();

	UFUNCTION()//statecomp
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

public:
	ACNPlayer(const FObjectInitializer& ObjectInitializer);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const;
	virtual void ChangeColor(FLinearColor InColor) override {};

	FORCEINLINE bool IsUnarmedMode() { return Action->IsUnarmedMode(); }

	void End_Roll();

	//FORCEINLINE class UCUserWidget_Select* GetSelectWidget() { return SelectWidget; }


	
};

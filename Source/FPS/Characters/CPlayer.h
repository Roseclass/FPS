#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "ICharacter.h"
#include "Components/TimelineComponent.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"

UCLASS()
class FPS_API ACPlayer : public ACharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(EditDefaultsOnly, Category = "TeamID")
		uint8 TeamID = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_Main> MainWidgetClass;

	//SceneComp
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Point;

	UPROPERTY(VisibleDefaultsOnly)
		class UPostProcessComponent* PostProcess;

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

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;

	class AController* DamageInstigator;

	int32 CurseCount = 3;
	bool Select = 0;

	//PostProcess TimeLine
	FTimeline StartCurseTimeline;
	FOnTimelineFloat StartCurseFloat;
	UPROPERTY(EditDefaultsOnly, Category = "Curse")
		class UCurveFloat* StartCurseCurve;

	FTimeline ReduceCurseTimeline;
	FOnTimelineFloat ReduceCurseFloat;
	UPROPERTY(EditDefaultsOnly, Category = "Curse")
		UCurveFloat* ReduceCurseCurve;

	FTimeline EndCurseTimeline;
	FOnTimelineFloat EndCurseFloat;
	UPROPERTY(EditDefaultsOnly, Category = "Curse")
		UCurveFloat* EndCurseCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Curse")
		class UMaterialParameterCollection* CurseMPC;

	UPROPERTY(EditDefaultsOnly, Category = "Curse")
		FName CurseParameterName;

	FTimeline ThermalTimeline;
	FOnTimelineFloat ThermalFloat;
	UPROPERTY(EditDefaultsOnly, Category = "Thermal")
		class UCurveFloat* ThermalCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Thermal")
		FName ThermalParameterName;

	class AActor* Causer;

	class IIInteract* InteractObject;


protected:	 
	UPROPERTY(BlueprintReadOnly)
		class UCUserWidget_Main* MainWidget;

	UPROPERTY(BlueprintReadWrite)
		bool OnWall = false;
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
	void OnInteract();
	void OffInteract();
	void OnThermal();
	void OffThermal();

	void StopOnWall();

	UFUNCTION()
		void EndEvade();

	void Begin_Roll();
	void Hitted();

	UFUNCTION()void Hitted_End();

	void Dead();
	void End_Dead();
	void Emotion();

	UFUNCTION() void OnHandGun();

	UFUNCTION() void OnReload();
	UFUNCTION() void OnSkill_1();
	UFUNCTION() void OnSkill_2();

	void OnDoAction();

	void OnAim();
	void OffAim();

	void OnSelect();
	void OffSelect();

	UFUNCTION()//statecomp
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//timeline
	UFUNCTION()	void EndingCurse(float Value);
	UFUNCTION()	void StartingCurse(float Value);
	UFUNCTION()	void ReducingCurse(float Value);
	UFUNCTION() void StartingThermal(float Value);

protected:
	virtual void BeginPlay() override;

public:
	ACPlayer();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const;
	virtual void ChangeColor(FLinearColor InColor) override {};
	UFUNCTION()virtual void End_Emotion() override;

	void End_Roll();

	//ACIneract_Wall->OnComponentBeginOverlap
	UFUNCTION()void BeginOnWall();

	void StartCurse();
	//ACInteract_Cure->OnComponentBeginOverlap
	UFUNCTION()void ReduceCurse();
	//EndCurseCurve->finish
	UFUNCTION()void EndCurse();

	void SetInteractObject(IIInteract* InObject);
	void ReleaseInteractObject(IIInteract* InObject);
};

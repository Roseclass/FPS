// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"
#include "Components/CStateComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Components/TimelineComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class FPS_API ACEnemy : public ACharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(EditAnywhere, Category = "DebugSettings")
		bool bDrawName = true;

	UPROPERTY(EditDefaultsOnly)
		float LaunchValue = 100.0f;

	UPROPERTY(EditDefaultsOnly)
		bool SuperArmor = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<AActor> DamageTextClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UCUserWidget_HealthBar> HealthBarClass;

	UCUserWidget_HealthBar* HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1;

	class AActor* Causer;
	class AController* DamageInstigator;

	FTimeline HideTimeline;
	FOnTimelineFloat HideFloat;
	UPROPERTY(EditDefaultsOnly, Category = "Hide")
		class UCurveFloat* HideCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Hide")
		class UMaterialParameterCollection* HideMPC;

	UPROPERTY(EditDefaultsOnly, Category = "Hide")
		FName HideParameterName;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActionComponent* Action;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCBoneComponent* Bone;

public:

//function
private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	void Hitted();
	void Dead();
	void Emotion();
	void End_Dead() override;
	void End_Emotion() override;

	UFUNCTION()void Hiding(float Value);

protected:
public:
	ACEnemy();

	virtual void OnConstruction(const FTransform& Transform)override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void ChangeColor(FLinearColor InColor) override {};
	virtual FGenericTeamId GetGenericTeamId() const;

	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

	void Hide();
	void Hide_End();
	void Show();
};

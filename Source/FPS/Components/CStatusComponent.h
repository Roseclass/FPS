// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterSpeed : uint8
{
	Walk, Run, Sprint, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAddHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSubHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UCUserWidget_PlayerInfo> PlayerInfoClass;
	UCUserWidget_PlayerInfo* PlayerInfo;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ECharacterSpeed::Max] = { 200, 400, 600 };

	float Health;

	bool bCanMove = true;

protected:
public:

//function
private:
protected:
	virtual void BeginPlay() override;

public:		
	UCStatusComponent();

	void SetMove();
	void SetStop();

	void SetSpeed(ECharacterSpeed InSpeed);

	void AddHealth(float InAmount);
	void SubHealth(float InAmount);

	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetHealthRatio() { return Health / MaxHealth <0.0f ? 0.0f : Health / MaxHealth; }

	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ECharacterSpeed::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ECharacterSpeed::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ECharacterSpeed::Sprint]; }

	FORCEINLINE bool CanMove() { return  bCanMove; }

	FAddHealth OnAddHealth;
	FSubHealth OnSubHealth;

};

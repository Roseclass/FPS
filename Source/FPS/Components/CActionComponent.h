#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, HandGun, OneHand, TwoHand, Warp, Tornado, MagicBall, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

//property
private:
	EActionType Type;

	UPROPERTY(EditDefaultsOnly)
		class UCActionData* DatasAssets[(int32)EActionType::Max];
	
	UPROPERTY()
		class UCAction* Datas[(int32)EActionType::Max];

protected:
public:
	UPROPERTY(BlueprintAssignable)//doaction_handgun->AbortByTypeChanged();
		FActionTypeChanged OnActionTypeChanged;

//function
private:
	void SetMode(EActionType InType);
	void ChangeType(EActionType InNewType);

protected:
	virtual void BeginPlay() override;

public:
	UCActionComponent();

	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCAction* GetCurrent() { return Datas[(int32)Type]; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsHandGunMode() { return Type == EActionType::HandGun; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsOneHandMode() { return Type == EActionType::OneHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTwoHandMode() { return Type == EActionType::TwoHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsWarpMode() { return Type == EActionType::Warp; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTornadoMode() { return Type == EActionType::Tornado; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsMagicBallMode() { return Type == EActionType::MagicBall; }

	UFUNCTION(BlueprintCallable) void SetUnarmedMode();
	UFUNCTION(BlueprintCallable) void SetHandGunMode();
	UFUNCTION(BlueprintCallable) void SetOneHandMode();
	UFUNCTION(BlueprintCallable) void SetTwoHandMode();
	UFUNCTION(BlueprintCallable) void SetWarpMode();
	UFUNCTION(BlueprintCallable) void SetTornadoMode();
	UFUNCTION(BlueprintCallable) void SetMagicBallMode();

	void OffAllCollision();

	void DoAction();
	void DoSkill(int32 InIdx);
	void Reload();

	void DoOnAim();
	void DoOffAim();

	void OnSelectMode();
	void OffSelectMode();
	bool CanSelect();

	void Dead();
	void End_Dead();

	void AbortByDamaged();
		
};

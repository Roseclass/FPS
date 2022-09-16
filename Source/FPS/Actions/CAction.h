#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Widgets/Layout/Anchors.h"
#include "Slate/WidgetTransform.h"
#include "CAction.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;



};

USTRUCT(BlueprintType)
struct FDoActionData : public FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Power = 5.0f;

	UPROPERTY(EditAnywhere)
		float HitStop;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditDefaultsOnly)
		FName BoneName;

	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<class UCameraShake> ShakeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
		UObject* Image;

	UPROPERTY(EditAnywhere)
		float CoolTime;

	UPROPERTY(EditAnywhere)
		float ThrowSpeed;
};

USTRUCT(BlueprintType)
struct FDoActionWidgetData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCUserWidget_WeaponInfo> WidgetClass;

	UPROPERTY(EditAnywhere)
		FAnchors Anchors;

	UPROPERTY(EditAnywhere)
		FVector2D Size;

	UPROPERTY(EditAnywhere)
		FWidgetTransform Transform;
};


UCLASS()
class FPS_API UCAction : public UObject
{
	GENERATED_BODY()
public:
	friend class UCActionData;
//property
private:
	class ACEquipment* Equipment;
	class ACAttachment* Attachment;
	class ACDoAction* DoAction;
	FLinearColor EquipmentColor;
protected:
public:

	//function
private:
protected:
public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class ACEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class ACDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE FLinearColor GetEquipmentColor() { return EquipmentColor; }
};

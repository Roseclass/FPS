#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "CProjectile_Path.generated.h"

UCLASS()
class FPS_API ACProjectile_Path : public AActor
{
	GENERATED_BODY()
//property
private:
	ACharacter* OwnerCharacter;
	AActor* OwnerProjectile;
	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;

	UPROPERTY(EditDefaultsOnly)
		FName BoneName;

	UPROPERTY(EditDefaultsOnly, Category = "SplineMesh")
		class UStaticMesh* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "SplineMesh")
		FPredictProjectilePathParams Input;

	UPROPERTY(EditDefaultsOnly, Category = "SplineMesh", meta = (ShortTooltip = "Material count should be same with Mesh"))
		TArray<class UMaterialInstanceConstant*> Materials;

	float Velocity = 1000.0f;
protected:
public:

//function
private:
protected:
	virtual void BeginPlay() override;

public:
	ACProjectile_Path();
	virtual void Tick(float DeltaTime) override;

	void SetInputVelocity(float InVelocity);
	void SetData(ACharacter* InCharacter, AActor* InProjectile);
	UFUNCTION(BlueprintImplementableEvent)
		void AddSplineMesh(FVector StartPos,FVector StartTan,FVector EndPos,FVector EndTan);
};

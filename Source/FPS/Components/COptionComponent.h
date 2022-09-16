// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "COptionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UCOptionComponent : public UActorComponent
{
	GENERATED_BODY()
//property
private:
protected:
public:
	UPROPERTY(EditAnywhere, Category = "Mouse")
		float HorizontalLookRate = 45.0f;

	UPROPERTY(EditAnywhere, Category = "Mouse")
		float VerticalLookRate = 45.0f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		float ZoomSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		FVector2D ZoomRange = FVector2D(0, 500);

//function
private:
protected:
	virtual void BeginPlay() override;

public:
	UCOptionComponent();

	FORCEINLINE float GetHorizontalLookRate() { return HorizontalLookRate; }
	FORCEINLINE float GetVerticalLookRate() { return VerticalLookRate; }

	FORCEINLINE float GetZoomSpeed() { return ZoomSpeed; }
	FORCEINLINE float GetMinZoomRange() { return ZoomRange.X; }
	FORCEINLINE float GetMaxZoomRange() { return ZoomRange.Y; }

};

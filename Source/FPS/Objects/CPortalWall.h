#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPortalWall.generated.h"

UCLASS()
class FPS_API ACPortalWall : public AActor
{
	GENERATED_BODY()
	
//property
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
		float WallWidth=100.0f;
	
	UPROPERTY(EditDefaultsOnly)
		float WallHeight=100.0f;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACPortal> PortalClass;

	TArray<AActor*> PortalsOnWall;
protected:
public:

//function
private:
	FVector2D ConstrainPortalToWall(float PortalY, float PortalZ);
	float ClampPointToWall(float Point, float WallSize, float PortalRadius);
	bool RectToRectCollision(FVector2D AOrigin, FVector2D AExtense, FVector2D BOrigin, FVector2D BExtense);
protected:
	virtual void BeginPlay() override;
public:
	ACPortalWall();
	virtual void OnConstruction(const FTransform& Transform)override;
	ACPortal* TryAddPortal(FVector PortalOrigin, bool IsPortalA);
	void OnPortalDestroyed(AActor* DestroyedActor);
	bool HasRoomForNewPortal(float NewPortalY,float NewPortalZ);
};

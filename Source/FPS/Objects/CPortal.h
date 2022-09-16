#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPortal.generated.h"

UCLASS()
class FPS_API ACPortal : public AActor
{
	GENERATED_BODY()
	//property
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* PortalBorderMesh;

	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent* PortalMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class USceneCaptureComponent2D* SceneCapture2D;

	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* BackFacingScene;

	UPROPERTY(EditAnywhere)
		class UMaterialInterface* BorderMaterialA;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* BorderMaterialB;

	UPROPERTY(EditAnywhere)
		class UTextureRenderTarget2D* TextureA;

	UPROPERTY(EditAnywhere)
		UTextureRenderTarget2D* TextureB;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* MeshMaterialA;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* MeshMaterialB;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* MeshMaterialNull;

	UPROPERTY(EditAnywhere)
		int32 UseCount=10;

	bool PortalA;
	ACPortal* LinkedPortal;
protected:
public:
	TArray<class ACharacter*> ActorsInPortal;
	//function
private:
	void TeleportActor(ACharacter* Target);

	UFUNCTION()
		void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void ActorDestroyed(AActor* DestroyedActor);

protected:
	virtual void BeginPlay() override;
public:
	ACPortal();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform)override;

	void LinkPortal(ACPortal* InPortal);
	void CheckIfActorShouldTeleport(ACharacter* Target);
	void SetIsPortalA(bool IsPortalA);
	void TickFunction(FVector InLocation, FRotator InRotation, float InCustomClipping);
	void SetUseCount(int32 InCount);
};

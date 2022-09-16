#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileBeginOverlap, FHitResult, InHitResult);

UCLASS()
class FPS_API ACProjectile : public AActor
{
	GENERATED_BODY()
//property
private:
	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
		class UParticleSystem* Explosion;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
		class UNiagaraSystem* NiagaraExplosion;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
		FTransform ExplosionTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
		TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
		float InnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
		float OutterRadius;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystemComponent* Particle;

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditDefaultsOnly)
		float Power = 0.0f;

	class APlayerController* Owner;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACEnemy> EnemyClass;

	UPROPERTY(BlueprintReadOnly)
		class ACEnemy* Target;
public:
	UPROPERTY(BlueprintAssignable)
		FProjectileBeginOverlap OnProjectileBeginOverlap;


//function
private:
protected:
	UFUNCTION()
		virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {};
	UFUNCTION()
		void SetSphereRadius(float InRadius);
	void DoExplosion();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	ACProjectile();
	void SettingProjectile(float InPower, APlayerController* InController,float InSpeed=-1.0f);
	void SettingTarget(ACEnemy* InTarget);

	virtual void SpawnChild() {};
};

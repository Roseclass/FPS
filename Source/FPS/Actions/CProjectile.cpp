#include "Actions/CProjectile.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Components/CBoneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GenericTeamAgentInterface.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

ACProjectile::ACProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Sphere, "Sphere");
	CHelpers::CreateComponent(this, &Particle, "Particle", Sphere);

	CHelpers::CreateActorComponent(this, &Projectile, "Projectile");

	InitialLifeSpan = 3.0f;

	Projectile->InitialSpeed = 4000.0f;
	Projectile->MaxSpeed = 8000.0f;
	Projectile->ProjectileGravityScale = 0.0f;
}

void ACProjectile::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACProjectile::OnComponentBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACProjectile::OnComponentEndOverlap);
}

void ACProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACProjectile::SettingProjectile(float InPower, APlayerController* InController, float InSpeed)
{
	Power = InPower;
	Owner = InController;
	if (InSpeed > 0.0f)
	{
		Projectile->InitialSpeed = InSpeed;
		Projectile->MaxSpeed = InSpeed;
		Projectile->ProjectileGravityScale = 0.49f;
	}
}

void ACProjectile::SettingTarget(ACEnemy* InTarget)
{
	Target = InTarget;
}

void ACProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == GetOwner());
	if(Cast<ACProjectile>(OtherActor))return;
	IGenericTeamAgentInterface* own = Cast<IGenericTeamAgentInterface>(GetOwner());
	IGenericTeamAgentInterface* other = Cast<IGenericTeamAgentInterface>(OtherActor);
	if (own && other)
	{
		if (own->GetGenericTeamId().GetId() == other->GetGenericTeamId().GetId())return;
	}

	if (!!Explosion)
	{
		FTransform transform = ExplosionTransform;
		transform.AddToTranslation(bFromSweep ? SweepResult.Location : GetActorLocation());
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, transform);
	}

	if (!!NiagaraExplosion)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraExplosion,
			bFromSweep ? SweepResult.Location : GetActorLocation(),
			SweepResult.ImpactNormal.Rotation(),
			FVector(1.0f));
	}

	if (!!CameraShakeClass)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeClass, GetActorLocation(), InnerRadius, OutterRadius, 1.0f, false);
	}

	FDamageEvent e; float rate = 1.0f;
	UCBoneComponent* bone = CHelpers::GetComponent<UCBoneComponent>(OtherActor);
	if (bone)rate = bone->GetRate(SweepResult.BoneName);
	OtherActor->TakeDamage(Power*rate, e, Owner, this);

	if (OnProjectileBeginOverlap.IsBound())
		OnProjectileBeginOverlap.Broadcast(SweepResult);

	Destroy();
}

void ACProjectile::SetSphereRadius(float InRadius)
{
	CLog::Print("in", 111);
	Sphere->SetSphereRadius(InRadius);
}
void ACProjectile::DoExplosion()
{
	if (!!Explosion)
	{
		FTransform transform = ExplosionTransform;
		transform.AddToTranslation(GetActorLocation());
		transform.SetRotation(FQuat(GetActorUpVector().Rotation()));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, transform);
	}

	if (!!NiagaraExplosion)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraExplosion,
			GetActorLocation(),
			GetActorUpVector().Rotation(),
			FVector(1.0f));
	}

	if (!!CameraShakeClass)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeClass, GetActorLocation(), InnerRadius, OutterRadius, 1.0f, false);
	}

}

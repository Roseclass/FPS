#include "Actions/CDoAction_Drongo_Throw.h"
#include "Global.h"
#include "CProjectile.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Actions/CProjectile_Path.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACDoAction_Drongo_Throw::ACDoAction_Drongo_Throw()
{
	CHelpers::GetClass<ACProjectile_Path>(&PathClass, "Blueprint'/Game/Enemies/Drongo/Projectiles/Grenade/BP_CProjectile_Path_Drongo_Throw.BP_CProjectile_Path_Drongo_Throw_C'");
}

void ACDoAction_Drongo_Throw::BeginPlay()
{
	Super::BeginPlay();
}

void ACDoAction_Drongo_Throw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDoAction_Drongo_Throw::DoAction()
{
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();

	ACProjectile_Path* path = GetWorld()->SpawnActorDeferred<ACProjectile_Path>(PathClass, FTransform(), OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	path->SetData(OwnerCharacter, nullptr);
	path->SetLifeSpan(3.0f);
	path->SetInputVelocity(Datas[0].ThrowSpeed);
	UGameplayStatics::FinishSpawningActor(path, FTransform());
}

void ACDoAction_Drongo_Throw::Begin_DoAction()
{
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(Datas[0].BoneName);
	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();
	rotator = UKismetMathLibrary::RLerp(UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetActorUpVector()),rotator,0.7,1);
	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	ACProjectile* projectileObject = GetWorld()->SpawnActorDeferred<ACProjectile>(Datas[0].ProjectileClass, transform, OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	//projectileObject->OnProjectileBeginOverlap.AddDynamic(this, &ACDoAction_HandGun::OnProjectileBeginOverlap);
	projectileObject->SettingProjectile(Datas[0].Power, OwnerCharacter->GetController<APlayerController>(), Datas[0].ThrowSpeed);
	UGameplayStatics::FinishSpawningActor(projectileObject, transform);
}

void ACDoAction_Drongo_Throw::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
}


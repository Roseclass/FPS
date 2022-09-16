#include "Actions/CDoAction_Drongo.h"
#include "Global.h"
#include "CProjectile.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

ACDoAction_Drongo::ACDoAction_Drongo()
{

}

void ACDoAction_Drongo::BeginPlay()
{
	Super::BeginPlay();
}

void ACDoAction_Drongo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDoAction_Drongo::DoAction()
{
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();

}

void ACDoAction_Drongo::Begin_DoAction()
{
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(Datas[0].BoneName);
	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();
	
	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	ACProjectile* projectileObject = GetWorld()->SpawnActorDeferred<ACProjectile>(Datas[0].ProjectileClass, transform, OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	//projectileObject->OnProjectileBeginOverlap.AddDynamic(this, &ACDoAction_HandGun::OnProjectileBeginOverlap);
	projectileObject->SettingProjectile(Datas[0].Power, OwnerCharacter->GetController<APlayerController>());
	UGameplayStatics::FinishSpawningActor(projectileObject, transform);
}

void ACDoAction_Drongo::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
}

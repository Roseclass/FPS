#include "Actions/CDoAction_Drongo_Gun.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "CProjectile.h"
#include "Characters/CAnimInstance.h"
#include "Components/CStatusComponent.h"
#include "Characters/CAIController.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Blueprint/UserWidget.h"

ACDoAction_Drongo_Gun::ACDoAction_Drongo_Gun()
{
	CHelpers::GetClass<UUserWidget>(&TargetingWidgetClass, "WidgetBlueprint'/Game/Widgets/Enemy/Drongo/WB_DrongoTargeting.WB_DrongoTargeting_C'");
}

void ACDoAction_Drongo_Gun::BeginPlay()
{
	Super::BeginPlay();
	Anim = Cast<UCAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	Controller = Cast<ACAIController>(OwnerCharacter->GetController());
	Behavior = CHelpers::GetComponent<UCBehaviorComponent>(Controller);
}

void ACDoAction_Drongo_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDoAction_Drongo_Gun::DoAction()
{
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
	Anim->SetAIUnarmedState(0);
	Anim->SetAIYaw(0.0f);
	Anim->SetAIPitch(0.0f);

	if (Behavior->GetTargetPlayer())
	{
		UUserWidget* widget;
		widget = CreateWidget<UUserWidget, APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0), TargetingWidgetClass);
		widget->AddToViewport();
		widget->SetVisibility(ESlateVisibility::HitTestInvisible);
		StartLockOn();
		ActionWidget = widget;
	}

	FTimerHandle WaitHandle;
	float WaitTime;
	WaitTime =3.0f;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
		EndLockOn();
	}), WaitTime, false);
	ActionTimerHandle = WaitHandle;
}

void ACDoAction_Drongo_Gun::Begin_DoAction()
{
	if (Behavior->GetTargetPlayer())ControlRot = UKismetMathLibrary::MakeRotFromX(Behavior->GetTargetPlayer()->GetActorLocation() - OwnerCharacter->GetActorLocation());	
	else ControlRot = Controller->GetControlRotation();
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(Datas[0].BoneName);	
	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(ControlRot));

	ACProjectile* projectileObject = GetWorld()->SpawnActorDeferred<ACProjectile>(Datas[0].ProjectileClass, transform, OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	projectileObject->OnProjectileBeginOverlap.AddDynamic(this, &ACDoAction_Drongo_Gun::OnProjectileBeginOverlap);
	projectileObject->SettingProjectile(Datas[0].Power,nullptr);
	UGameplayStatics::FinishSpawningActor(projectileObject, transform);
}

void ACDoAction_Drongo_Gun::End_DoAction()
{
	State->SetIdleMode();
	Anim->SetAIUnarmedState(1);
	Status->SetMove();
}

void ACDoAction_Drongo_Gun::StartLockOn()
{
	LockOn = 1;
}

void ACDoAction_Drongo_Gun::EndLockOn()
{
	LockOn = 0;
}

void ACDoAction_Drongo_Gun::OnProjectileBeginOverlap(FHitResult InHitResult)
{
	//UProceduralMeshComponent* otherProcMesh = Cast<UProceduralMeshComponent>(InHitResult.GetComponent());
	//
	//if (!!otherProcMesh)
	//{
	//	FVector planeNormals[2] = { GetActorUpVector(), GetActorRightVector() };
	//	UProceduralMeshComponent* outProcMesh = nullptr;
	//
	//	UMaterialInstanceConstant* material;
	//	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/MAT_Slice_Inst.MAT_Slice_Inst'");
	//
	//	UKismetProceduralMeshLibrary::SliceProceduralMesh
	//	(
	//		otherProcMesh,
	//		InHitResult.Location,
	//		planeNormals[UKismetMathLibrary::RandomIntegerInRange(0, 1)],
	//		true,
	//		outProcMesh,
	//		EProcMeshSliceCapOption::CreateNewSectionForCap,
	//		material
	//	);
	//
	//	outProcMesh->SetSimulatePhysics(true);
	//	outProcMesh->AddImpulse(FVector(1000.0f, 1000.0f, 1000.0f), NAME_None, true);
	//}
}

void ACDoAction_Drongo_Gun::Abort()
{
	CLog::Print("in");
	GetWorld()->GetTimerManager().ClearTimer(ActionTimerHandle);
	EndLockOn();
	End_DoAction();
	if(ActionWidget)ActionWidget->SetVisibility(ESlateVisibility::Hidden);
}
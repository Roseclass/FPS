#include "Actions/CDoAction_HandGun.h"
#include "Global.h"
#include "Characters/CSliceable.h"
#include "Characters/CEnemy_Decoy.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CBoneComponent.h"
#include "CAim.h"
#include "CProjectile.h"
#include "Widgets/CUserWidget_WeaponInfo.h"
#include "Widgets/CUserWidget_SkillSlot.h"
#include "GameFramework/Character.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/CanvasPanelSlot.h"
#include "Particles/ParticleSystemComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Objects/CSliceableParts.h"


ACDoAction_HandGun::ACDoAction_HandGun()
{
	CHelpers::GetClass<UCUserWidget_Select>(&SelectWidgetClass, "WidgetBlueprintGeneratedClass'/Game/Widgets/WB_SelectSkill.WB_SelectSkill_C'");
	CHelpers::GetClass<UCUserWidget_SkillSlot>(&SlotWidgetClass, "WidgetBlueprintGeneratedClass'/Game/Widgets/WB_Player_SkillSlot.WB_Player_SkillSlot_C'");
	CHelpers::GetAsset<UParticleSystem>(&Particle, "ParticleSystem'/Game/Player/Particles/P_Revenant_Primary_HitCharacter_2.P_Revenant_Primary_HitCharacter_2'");

	WidgetTransforms.Add(FWidgetTransform(FVector2D(-250.0f, -100.0f), FVector2D(1.0f, 1.0f), FVector2D(0.0f, 0.0f), 0.0f));
	WidgetTransforms.Add(FWidgetTransform(FVector2D(-400.0f, -100.0f), FVector2D(1.0f, 1.0f), FVector2D(0.0f, 0.0f), 0.0f));

	WidgetDataIndices.Add(1);
	WidgetDataIndices.Add(2);

}

void ACDoAction_HandGun::BeginPlay()
{
	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);

	Action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	Action->OnActionTypeChanged.AddDynamic(this, &ACDoAction_HandGun::AbortByTypeChanged);

	if (WidgetDatas.Num()>0)
	{
		InfoWidget = CreateWidget<UCUserWidget_WeaponInfo, APlayerController>(OwnerCharacter->GetController<APlayerController>(), WidgetDatas[0].WidgetClass);
		InfoWidget->AddToViewport();
		InfoWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		InfoWidget->SetRenderTransform(WidgetDatas[0].Transform);
		InfoWidget->SetCurText(MaxCount);
		InfoWidget->SetWholeText(MaxCount);
	}
	
	for (int32 i = 0; i < WidgetTransforms.Num(); i++)
	{
		UCUserWidget_SkillSlot* temp;
		temp = CreateWidget<UCUserWidget_SkillSlot, APlayerController>(OwnerCharacter->GetController<APlayerController>(), SlotWidgetClass);
		temp->AddToViewport();
		temp->SetVisibility(ESlateVisibility::HitTestInvisible);
		temp->SetRenderTransform(WidgetTransforms[i]);
		SlotWidget.Add(temp);
		OnSelectModeOn.AddDynamic(SlotWidget[i], &UCUserWidget_SkillSlot::Pause);
		OnSelectModeOff.AddDynamic(SlotWidget[i], &UCUserWidget_SkillSlot::Resume);
		SetSlot(i);
	}

	if (SelectWidgetClass)
	{
		SelectWidget = CreateWidget<UCUserWidget_Select, APlayerController>(OwnerCharacter->GetController<APlayerController>(), SelectWidgetClass);
		SelectWidget->AddToViewport();
		SelectWidget->SetVisibility(ESlateVisibility::Collapsed);
		OnSelectModeOn.AddDynamic(SelectWidget, &UCUserWidget_Select::PlayOnSequence);
		OnSelectModeOff.AddDynamic(SelectWidget, &UCUserWidget_Select::PlayOffSequence);
		SelectWidget->OnChangeSlotData.AddDynamic(this, &ACDoAction_HandGun::ChangeSlotData);
		for (int32 i = 0; i <4; i++)
		{
			if (Datas.Num() <= i+2)break;
			if (Datas[i+2].Image)
				SelectWidget->SetData(i, i+2, Datas[i+2].Image);
		}
	}

}

void ACDoAction_HandGun::DoAction()
{
	CheckFalse(State->IsIdleMode());
	if (!Count)
	{
		Reload();
		return;
	}
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_HandGun::Begin_DoAction()
{
	//if (Aim->IsAvaliable())
	//CheckFalse(Aim->InZoom());
	//FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(Datas[0].BoneName);
	//FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();
	//
	//FTransform transform = Datas[0].EffectTransform;
	//transform.AddToTranslation(location);
	//transform.SetRotation(FQuat(rotator));
	
	//ACProjectile* projectileObject = GetWorld()->SpawnActorDeferred<ACProjectile>(Datas[0].ProjectileClass, transform, OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	//projectileObject->OnProjectileBeginOverlap.AddDynamic(this, &ACDoAction_HandGun::OnProjectileBeginOverlap);
	//projectileObject->SettingProjectile(Datas[0].Power, OwnerCharacter->GetController<APlayerController>());
	//UGameplayStatics::FinishSpawningActor(projectileObject, transform);

	Count--; InfoWidget->SetCurText(Count);
	FVector2D vec;	FVector wp, wd;
	if (GEngine && GEngine->GameViewport)	GEngine->GameViewport->GetViewportSize(vec);
	vec.X /= 2;	vec.Y /= 2;
	UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0),	vec, wp, wd);
	TArray<AActor*> actors;
	FHitResult result;
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		wp, wp + UKismetMathLibrary::Normal(wd) * 10000.0f,
		ETraceTypeQuery::TraceTypeQuery1,
		0,
		actors,
		EDrawDebugTrace::None,
		result,
		1))
	{
		ACEnemy_Decoy* decoy = Cast<ACEnemy_Decoy>(result.GetActor());
		if (decoy)
		{
			decoy->DoPierce(result.Location);
			return;
		}

		ACSliceable* slice = Cast<ACSliceable>(result.GetActor());
		if (slice)
		{			
			slice->DoSlice(result.BoneName,result.Location, EProcMeshSliceCapOption::CreateNewSectionForCap, UKismetMathLibrary::Normal(result.TraceStart-result.TraceEnd));
			return;
		}

		ACSliceableParts* sliceParts = Cast<ACSliceableParts>(result.GetActor());
		if (sliceParts)
		{
			sliceParts->Slice(result.Location, GetActorUpVector(), 1, EProcMeshSliceCapOption::CreateNewSectionForCap);
			return;
		}

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, result.ImpactPoint,UKismetMathLibrary::MakeRotFromX(result.Normal),FVector(0.7f));
		IGenericTeamAgentInterface* own = Cast<IGenericTeamAgentInterface>(OwnerCharacter);
		IGenericTeamAgentInterface* other = Cast<IGenericTeamAgentInterface>(result.GetActor());
		if (own && other)
		{
			if (own->GetGenericTeamId().GetId() != other->GetGenericTeamId().GetId())
			{
				FDamageEvent e; float rate = 1.0f;
				UCBoneComponent* bone = CHelpers::GetComponent<UCBoneComponent>(result.GetActor());
				if (bone)
				{
					rate = bone->GetRate(result.BoneName);
					bone->SetCurrentHittedBone(result.BoneName);
				}
				result.GetActor()->TakeDamage(Datas[0].Power * rate, e, OwnerCharacter->GetController(), this);
			}
		}
	}
}

void ACDoAction_HandGun::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_HandGun::DoSkill(int32 InIdx)
{
	CheckFalse(State->IsIdleMode());
	if (InIdx >= WidgetTransforms.Num())
	{
		CLog::Print(InIdx);
		return;
	}
	if (!SlotWidget[InIdx]->IsCanUse())
	{
		SlotWidget[InIdx]->PlayCantUse();
		return;
	}
	CurrentSlot = InIdx;
	int32 idx = WidgetDataIndices[CurrentSlot];
	
	if (Datas[idx].AnimMontage)
	{
		State->SetActionMode();
		OwnerCharacter->PlayAnimMontage(Datas[idx].AnimMontage, Datas[idx].PlayRate, Datas[idx].StartSection);
		Datas[idx].bCanMove ? Status->SetMove() : Status->SetStop();
	}
	SlotWidget[CurrentSlot]->PlayCoolDown();
}

void ACDoAction_HandGun::Begin_DoSkill()
{
	int32 idx = WidgetDataIndices[CurrentSlot];

	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(Datas[idx].BoneName);
	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();

	FTransform transform = Datas[idx].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));
	
	ACProjectile* projectileObject = GetWorld()->SpawnActorDeferred<ACProjectile>(Datas[idx].ProjectileClass, transform, OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	projectileObject->OnProjectileBeginOverlap.AddDynamic(this, &ACDoAction_HandGun::OnProjectileBeginOverlap);
	projectileObject->SettingProjectile(Datas[idx].Power, OwnerCharacter->GetController<APlayerController>());
	UGameplayStatics::FinishSpawningActor(projectileObject, transform);
	projectileObject->SpawnChild();
}

void ACDoAction_HandGun::End_DoSkill()
{

}

void ACDoAction_HandGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Aim->Tick(DeltaTime);
}

void ACDoAction_HandGun::Reload()
{
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();
	if(!ReloadData.AnimMontage)
	{CLog::Print(ReloadData.AnimMontage->GetName()); }
	OwnerCharacter->PlayAnimMontage(ReloadData.AnimMontage, ReloadData.PlayRate, ReloadData.StartSection);

	ReloadData.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_HandGun::Begin_Reload()
{

}

void ACDoAction_HandGun::End_Reload()
{
	InfoWidget->SetCurText(MaxCount);
	Count = MaxCount;
	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_HandGun::OnAim()
{
	Aim->On();
}

void ACDoAction_HandGun::OffAim()
{
	Aim->Off();
}

void ACDoAction_HandGun::OnProjectileBeginOverlap(FHitResult InHitResult)
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

void ACDoAction_HandGun::AbortByTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	//CheckFalse(Aim->IsAvaliable());
	//CheckFalse(Aim->InZoom())
	//Aim->Off();
}

void ACDoAction_HandGun::SetSlot(int32 SlotIndex)
{
	int32 idx = WidgetDataIndices[SlotIndex];
	SlotWidget[SlotIndex]->SetData(Datas[idx].Image, Datas[idx].CoolTime);
}

void ACDoAction_HandGun::ChangeSlotData(int32 SlotIndex)
{
	if (!SlotWidget[SlotIndex]->IsCanUse())return;
	int32 idx = SelectWidget->GetDataIndex();
	if (idx == -1)return;
	WidgetDataIndices[SlotIndex] = idx;
	SetSlot(SlotIndex);
}

void ACDoAction_HandGun::OnSelectMode()
{
	if (OnSelectModeOn.IsBound())
		OnSelectModeOn.Broadcast();
}

void ACDoAction_HandGun::OffSelectMode()
{
	if (OnSelectModeOff.IsBound())
		OnSelectModeOff.Broadcast();
}

bool ACDoAction_HandGun::CanSelect()
{
	if (!SlotWidget[1]->IsCanUse())return 0;
	return SelectWidget->CanSelect();
}
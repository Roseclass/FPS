#include "Components/CActionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actions/CAction.h"
#include "Actions/CActionData.h"
#include "Actions/CEquipment.h"
#include "Actions/CAttachment.h"
#include "Actions/CDoAction.h"

UCActionComponent::UCActionComponent()
{

}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!DatasAssets[i])
			DatasAssets[i]->BeginPlay(character, &Datas[i]);
	}
}


void UCActionComponent::SetUnarmedMode()
{
	if (!!Datas[(int32)Type])
		Datas[(int32)Type]->GetEquipment()->Unequip();
	
	Datas[(int32)EActionType::Unarmed]->GetEquipment()->Equip();

	ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetHandGunMode()
{
	SetMode(EActionType::HandGun);
}

void UCActionComponent::SetOneHandMode()
{
	SetMode(EActionType::OneHand);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetWarpMode()
{
	SetMode(EActionType::Warp);
}

void UCActionComponent::SetTornadoMode()
{
	SetMode(EActionType::Tornado);
}

void UCActionComponent::SetMagicBallMode()
{
	SetMode(EActionType::MagicBall);
}

void UCActionComponent::OffAllCollision()
{
	for (UCAction* data : Datas)
	{
		if (!!data == false)
			continue;
	
		if (!!data->GetAttachment() == false)
			continue;
	
		data->GetAttachment()->OffCollision();
	}
}


void UCActionComponent::SetMode(EActionType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();
		return;
	}
	
	else if (IsUnarmedMode() == false)
	{
		if (!!Datas[(int32)Type])
			Datas[(int32)Type]->GetEquipment()->Unequip();
	}
	
	if (!!Datas[(int32)InType] && !!Datas[(int32)InType]->GetEquipment())
		Datas[(int32)InType]->GetEquipment()->Equip();

	ChangeType(InType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType prevType = Type;
	Type = InNewType;




	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType, Type);
}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();
	
		if (!!action)
			action->DoAction();
	}
}

void UCActionComponent::DoSkill(int32 InIdx)
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action)
			action->DoSkill(InIdx);
	}
}

void UCActionComponent::Reload()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action)
			action->Reload();
	}
}

void UCActionComponent::DoOnAim()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();
	
		if (!!action)
			action->OnAim();
	}
}

void UCActionComponent::DoOffAim()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();
	
		if (!!action)
			action->OffAim();
	}
}

void UCActionComponent::OnSelectMode()
{
	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action)
			action->OnSelectMode();
	}
}

void UCActionComponent::OffSelectMode()
{
	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action)
			action->OffSelectMode();
	}
}

bool UCActionComponent::CanSelect()
{
	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action)
			return action->CanSelect();
	}
	return 0;
}

void UCActionComponent::Dead()
{
	OffAllCollision();
}

void UCActionComponent::End_Dead()
{
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!Datas[i] && !!Datas[i]->GetAttachment())
			Datas[i]->GetAttachment()->Destroy();
	
		if (!!Datas[i] && !!Datas[i]->GetEquipment())
			Datas[i]->GetEquipment()->Destroy();
	
		if (!!Datas[i] && !!Datas[i]->GetDoAction())
			Datas[i]->GetDoAction()->Destroy();
	}
}

void UCActionComponent::AbortByDamaged()
{
	CheckNull(Datas[(int32)Type]);
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type]->GetEquipment())
	{
		Datas[(int32)Type]->GetEquipment()->Begin_Equip();
		Datas[(int32)Type]->GetEquipment()->End_Equip();
	}
	
	if (!!Datas[(int32)Type]->GetDoAction())
		Datas[(int32)Type]->GetDoAction()->Abort();

}

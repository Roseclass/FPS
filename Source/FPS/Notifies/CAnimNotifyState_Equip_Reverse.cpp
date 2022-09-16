#include "Notifies/CAnimNotifyState_Equip_Reverse.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Actions/CEquipment.h"

FString UCAnimNotifyState_Equip_Reverse::GetNotifyName_Implementation() const
{
	return "Equip_Reverse";
}

void UCAnimNotifyState_Equip_Reverse::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrent()->GetEquipment()->End_Equip();
}

void UCAnimNotifyState_Equip_Reverse::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrent()->GetEquipment()->Begin_Equip();
}

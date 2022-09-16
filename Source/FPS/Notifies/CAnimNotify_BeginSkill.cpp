#include "Notifies/CAnimNotify_BeginSkill.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"

FString UCAnimNotify_BeginSkill::GetNotifyName_Implementation() const
{
	return "BeginSkill";
}

void UCAnimNotify_BeginSkill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);
	action->GetCurrent()->GetDoAction()->Begin_DoSkill();
}

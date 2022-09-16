#include "Notifies/CAnimNotify_EndDead.h"
#include "Global.h"
#include "Characters/ICharacter.h"

FString UCAnimNotify_EndDead::GetNotifyName_Implementation() const
{
	return "EndDead";
}

void UCAnimNotify_EndDead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	character->End_Dead();
}

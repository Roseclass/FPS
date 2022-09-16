#include "Notifies/CAnimNotify_Roll_End.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_Roll_End::GetNotifyName_Implementation() const
{
	return "Roll_End";
}

void UCAnimNotify_Roll_End::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_Roll();
}

#include "Notifies/CAnimNotify_EndEmotion.h"
#include "Global.h"
#include "Characters/ICharacter.h"

FString UCAnimNotify_EndEmotion::GetNotifyName_Implementation() const
{
	return "Emotion_End";
}

void UCAnimNotify_EndEmotion::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	character->End_Emotion();
}


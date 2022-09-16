#include "Notifies/CAnimNotifyState_Turn.h"
#include "Global.h"
#include "GameFramework/Character.h"

FString UCAnimNotifyState_Turn::GetNotifyName_Implementation() const
{
	return "Turn";
}

void UCAnimNotifyState_Turn::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	Character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(Character);
	Current = Character->GetActorRotation();
	//FRotator rAim = Character->GetBaseAimRotation();
	//FVector vAim = UKismetMathLibrary::GetForwardVector(rAim);
	//float result = UKismetMathLibrary::Dot_VectorVector(vAim, Character->GetActorRightVector());
	//FVector right = Character->GetActorRightVector();
	//if (result < 0.0f) right *= -1.0f;
	//Target = UKismetMathLibrary::MakeRotFromXZ(right, Character->GetActorUpVector());
	////CLog::Print(result, 1);
	////CLog::Print(right, 2);
	////CLog::Print(Target, 3);
	//CLog::Print("In");
}
void UCAnimNotifyState_Turn::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	CheckNull(Character);
	FRotator rot; FVector vec;
	rot = Character->GetControlRotation();
	vec = UKismetMathLibrary::GetForwardVector(rot);
	vec = UKismetMathLibrary::ProjectVectorOnToPlane(vec, Character->GetActorUpVector());
	vec = UKismetMathLibrary::Normal(vec);
	Target = UKismetMathLibrary::MakeRotFromXZ(vec,Character->GetActorUpVector());
	Current = UKismetMathLibrary::RLerp(Current, Target, 0.05f, 1);
	Character->SetActorRotation(Current);
}

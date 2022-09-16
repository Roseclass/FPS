#include "Components/CBehaviorComponent.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Characters/CAnimInstance.h"
#include "Characters/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBehaviorComponent::UCBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
	Controller = Cast<ACAIController>(GetOwner());
	Character = Cast<ACharacter>(Controller->GetPawn());
	Anim = Cast<UCAnimInstance>(Character->GetMesh()->GetAnimInstance());
}

void UCBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckNull(GetTargetPlayer());
	CalculateYawPitch();
}

void UCBehaviorComponent::Init()
{
	Blackboard->SetValueAsInt(ActCount, -1);
}

bool UCBehaviorComponent::IsWaitMode()
{
	return GetType() == EBehaviorType::Wait;
}

bool UCBehaviorComponent::IsApproachMode()
{
	return GetType() == EBehaviorType::Approach;
}

bool UCBehaviorComponent::IsActionMode()
{
	return GetType() == EBehaviorType::Action;
}

bool UCBehaviorComponent::IsAvoidMode()
{
	return GetType() == EBehaviorType::Avoid;
}

bool UCBehaviorComponent::IsSpecialMode()
{
	return GetType() == EBehaviorType::Special;
}

void UCBehaviorComponent::SetWaitMode()
{
	ChangeType(EBehaviorType::Wait);
}

void UCBehaviorComponent::SetApproachMode()
{
	ChangeType(EBehaviorType::Approach);
}

void UCBehaviorComponent::SetActionMode()
{
	ChangeType(EBehaviorType::Action);
}

void UCBehaviorComponent::SetAvoidMode()
{
	ChangeType(EBehaviorType::Avoid);
}

void UCBehaviorComponent::SetSpecialMode()
{
	ChangeType(EBehaviorType::Special);
}

ACPlayer* UCBehaviorComponent::GetTargetPlayer()
{
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

FVector UCBehaviorComponent::GetWarpLocation()
{
	return Blackboard->GetValueAsVector(WarpLocation);
}

void UCBehaviorComponent::ChangeType(EBehaviorType InNewType)
{
	EBehaviorType type = GetType();
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InNewType);

	if (OnBehaviorTypeChanged.IsBound())
		OnBehaviorTypeChanged.Broadcast(type, InNewType);
}

EBehaviorType UCBehaviorComponent::GetType()
{
	return (EBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}

void UCBehaviorComponent::IncreaseActCount(bool IsRepeat)
{
	int32 cur = Blackboard->GetValueAsInt(ActCount) + 1;
	if(IsRepeat)cur %= MaxActCount;
	Blackboard->SetValueAsInt(ActCount,cur);
}

void UCBehaviorComponent::CalculateYawPitch()
{
	CheckNull(Anim);
	FRotator cur = GetOwner()->GetActorRotation();
	ControlRot = UKismetMathLibrary::MakeRotFromX(GetTargetPlayer()->GetActorLocation() - Character->GetActorLocation());
	FRotator rot = UKismetMathLibrary::NormalizedDeltaRotator(ControlRot, cur);
	Anim->SetAIYaw(rot.Yaw);
	Anim->SetAIPitch(rot.Pitch);
	Controller->SetControlRotation(ControlRot);
}

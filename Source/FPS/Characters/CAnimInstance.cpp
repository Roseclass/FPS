#include "Characters/CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CActionComponent.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);
	PrevRot = character->GetActorRotation();

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	action->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChaged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	IsInAir = character->GetMovementComponent()->IsFalling();

	Speed = character->GetVelocity().Size2D();

	if (!AI)
	{
		character->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0.0f ? IsAccelerating = 1 : IsAccelerating = 0;

		FRotator cur = character->GetActorRotation();
		FRotator aim = character->GetBaseAimRotation();
		FRotator rot = UKismetMathLibrary::NormalizedDeltaRotator(aim, cur);

		float dir = UKismetMathLibrary::NormalizedDeltaRotator(aim, UKismetMathLibrary::MakeRotFromX(character->GetVelocity())).Yaw;
		if (abs(dir) > 135.0f)
		{
			if (abs(Direction) < 135.0f)	Direction = 135.0f;
			if (dir * Direction < 0.0f)	Direction *= -1.0f;
		}
		Direction = FMath::FInterpTo(Direction, dir, DeltaSeconds, 10.0f);

		Yaw = rot.Yaw * !character->bUseControllerRotationYaw; Pitch = rot.Pitch; Roll = rot.Roll;

		rot = UKismetMathLibrary::NormalizedDeltaRotator(PrevRot, cur);

		PrevRot = cur;
		IsUnarmed = CHelpers::GetComponent<UCActionComponent>(character)->IsUnarmedMode();
	}

	//YawDelta = UKismetMathLibrary::FInterpTo(rot.Yaw, rot.Yaw /(DeltaSeconds* LeanIntensity), DeltaSeconds, InterpSpeed);
	
	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(character);
	if (!!feet)
	{
		FeetData = feet->GetData();
		bIkMode = feet->GetIkMode();
	}
}

void UCAnimInstance::OnActionTypeChaged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}

void UCAnimInstance::SetAIAccelerating(bool Inbool)
{
	CheckFalse(AI);
	IsAccelerating = Inbool;
}

void UCAnimInstance::SetAIUnarmedState(bool Inbool)
{
	CheckFalse(AI);
	IsUnarmed = Inbool;
}

void UCAnimInstance::SetAIYaw(float InYaw)
{
	CheckFalse(AI);
	Yaw = InYaw;
}

void UCAnimInstance::SetAIPitch(float InPitch)
{
	CheckFalse(AI);
	Pitch = InPitch;
}

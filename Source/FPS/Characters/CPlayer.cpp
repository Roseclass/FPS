#include "Characters/CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Widgets/CUserWidget_Main.h"
#include "Objects/IInteract.h"
#include "Components/CFeetComponent.h"
#include "Characters/CEnemy.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create SceneComponent
	CHelpers::CreateComponent(this, &Point, "Point", GetCapsuleComponent());
	CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);
	CHelpers::CreateComponent(this, &PostProcess, "PostProcess", GetRootComponent());

	//Create ActorComponent
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Option, "Option");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Feet, "Feet");

	//Component Settings
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Player/Animation/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	SpringArm->SetRelativeLocation(FVector(0, 0, 100));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;

	Point->SetRelativeLocation(FVector(0, 0, -70));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	PostProcess->bEnabled = true;
	FWeightedBlendable curse;
	UMaterialInstanceConstant* cptr;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&cptr, "MaterialInstanceConstant'/Game/Materials/Curse/PP_Curse_Inst.PP_Curse_Inst'");
	curse.Object = cptr;
	curse.Weight = 0.0f;
	PostProcess->Settings.WeightedBlendables.Array.Add(curse);

	FWeightedBlendable theraml;
	UMaterialInstanceConstant* tptr;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&tptr, "MaterialInstanceConstant'/Game/Materials/Thermal/PP_Thermal_Inst.PP_Thermal_Inst'");
	theraml.Object = tptr;
	theraml.Weight = 0.0f;
	PostProcess->Settings.WeightedBlendables.Array.Add(theraml);

}


void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	Action->SetUnarmedMode();

	MainWidget = CreateWidget<UCUserWidget_Main, APlayerController>(GetController<APlayerController>(), MainWidgetClass);
	MainWidget->AddToViewport();
	MainWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACPlayer::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACPlayer::OnEndOverlap);

	//PostProcessTimeLine
	StartCurseFloat.BindUFunction(this, "StartingCurse");
	StartCurseTimeline.AddInterpFloat(StartCurseCurve, StartCurseFloat);
	//StartCurseTimeline.SetPlayRate(200.0f);

	ReduceCurseFloat.BindUFunction(this, "ReducingCurse");
	ReduceCurseTimeline.AddInterpFloat(ReduceCurseCurve, ReduceCurseFloat);

	EndCurseFloat.BindUFunction(this, "EndingCurse");
	EndCurseTimeline.AddInterpFloat(EndCurseCurve, EndCurseFloat);

	ThermalFloat.BindUFunction(this,"StartingThermal");
	ThermalTimeline.AddInterpFloat(ThermalCurve, ThermalFloat);

	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(Cast<UMaterialInstanceConstant>(PostProcess->Settings.WeightedBlendables.Array[1].Object), this);
	PostProcess->Settings.WeightedBlendables.Array[1].Object = DynMaterial;
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StartCurseTimeline.TickTimeline(DeltaTime);
	ReduceCurseTimeline.TickTimeline(DeltaTime);
	EndCurseTimeline.TickTimeline(DeltaTime);
	ThermalTimeline.TickTimeline(DeltaTime);

	if (FMath::IsNearlyZero(GetInputAxisValue("MoveForward"))&&FMath::IsNearlyZero(GetInputAxisValue("MoveRight")))
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	if (OnWall)
	{
		TArray<AActor*> arr;
		FVector start, end;
		start = Point->GetComponentLocation();
		end = GetActorUpVector()*(GetCapsuleComponent()->GetScaledCapsuleHalfHeight())*2.0f;
		FHitResult DResult,FResult;
		UKismetSystemLibrary::LineTraceSingle
		(
			GetWorld(),
			start,
			start - end,
			ETraceTypeQuery::TraceTypeQuery1,
			0,
			arr,
			EDrawDebugTrace::ForDuration,
			DResult,
			1,
			FLinearColor::Blue
		);
		start += GetActorForwardVector()*5.0f;
		bool check =
		UKismetSystemLibrary::LineTraceSingle
		(
			GetWorld(),
			start,
			start - end,
			ETraceTypeQuery::TraceTypeQuery1,
			0,
			arr,
			EDrawDebugTrace::ForDuration,
			FResult,
			1
		);
		if (!check)StopOnWall();
		else
		{
			FVector loc; FRotator rot;
			loc = DResult.Location + DResult.Normal * (GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			rot = UKismetMathLibrary::MakeRotFromXZ(UKismetMathLibrary::Normal(FResult.Location - DResult.Location), DResult.Normal);
			SetActorLocationAndRotation(loc, rot);
		}
	}
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ACPlayer::OnWalk);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, this, &ACPlayer::OffWalk);

	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACPlayer::OnJump);

	PlayerInputComponent->BindAction("HandGun", EInputEvent::IE_Pressed, this, &ACPlayer::OnHandGun);

	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &ACPlayer::OnReload);
	PlayerInputComponent->BindAction("Skill_1", EInputEvent::IE_Pressed, this, &ACPlayer::OnSkill_1);
	PlayerInputComponent->BindAction("Skill_2", EInputEvent::IE_Pressed, this, &ACPlayer::OnSkill_2);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);

	PlayerInputComponent->BindAction("Select", EInputEvent::IE_Pressed, this, &ACPlayer::OnSelect);
	PlayerInputComponent->BindAction("Select", EInputEvent::IE_Released, this, &ACPlayer::OffSelect);

	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ACPlayer::OnInteract);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Released, this, &ACPlayer::OffInteract);

	PlayerInputComponent->BindAction("Thermal", EInputEvent::IE_Pressed, this, &ACPlayer::OnThermal);

}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACPlayer::OnMoveForward(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rot;FVector direction;
	rot = GetControlRotation();
	if (!OnWall)
	{
		rot.Roll = 0.0f; rot.Pitch = 0.0f;
		direction = UKismetMathLibrary::GetForwardVector(rot);
	}
	else
	{
		direction = UKismetMathLibrary::GetForwardVector(rot);
		direction = UKismetMathLibrary::ProjectVectorOnToPlane(direction, GetActorUpVector());
		direction = UKismetMathLibrary::Normal(direction);
	}
	AddMovementInput(direction, InAxis);	
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rot = GetControlRotation();
	rot.Pitch = 0.0f;
	FVector direction = UKismetMathLibrary::GetRightVector(rot);
	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
	//AddControllerYawInput(InAxis);
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
	//AddControllerPitchInput(InAxis);
}

void ACPlayer::OnZoom(float InAxis)
{
	SpringArm->TargetArmLength -= (InAxis * Option->GetZoomSpeed() * GetWorld()->GetDeltaSeconds());
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, Option->GetMinZoomRange(), Option->GetMaxZoomRange());
}

void ACPlayer::OnWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	
}

void ACPlayer::OffWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
}

void ACPlayer::OnEvade()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Status->CanMove());

	/*if (Action->IsUnarmedMode())
	{
		GetCharacterMovement()->GravityScale = 0.0f;

		FVector direction = FVector::ZeroVector;
		if (FMath::IsNearlyZero(GetVelocity().Size()))
			direction = GetActorUpVector();
		else
			direction = GetVelocity().GetSafeNormal();

		FVector launch = direction * GetCharacterMovement()->MaxWalkSpeed * 0.5f;

		LaunchCharacter(launch, false, true);
		SpringArm->TargetArmLength = 300.0f;
		UKismetSystemLibrary::K2_SetTimer(this, "EndEvade", 1.0f, false);
		return;
	}


	if (InputComponent->GetAxisValue("MoveForward") < 0.0f)
	{
		State->SetBackStepMode();
		return;
	}*/

	State->SetRollMode();
}

void ACPlayer::EndEvade()
{
	GetCharacterMovement()->GravityScale = 1.0f;
	SpringArm->TargetArmLength = 200.0f;
}

void ACPlayer::OnJump()
{
	if (OnWall)StopOnWall();
	else Jump();
}

void ACPlayer::OnInteract()
{
	CheckNull(InteractObject);
	InteractObject->Interact();
	Emotion();
}

void ACPlayer::OffInteract()
{
	CheckNull(InteractObject);
	InteractObject->Interact_End();
	if (GetCurrentMontage())StopAnimMontage();
	End_Emotion();
}

void ACPlayer::OnThermal()
{
	PostProcess->Settings.WeightedBlendables.Array[1].Weight=1.0f;
	FTimerHandle WaitHandle;
	float WaitTime;
	WaitTime = 10.0f;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		OffThermal();
	}), WaitTime, false);
	TArray<AActor*> arr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACEnemy::StaticClass(), arr);
	for (AActor* i : arr)
	{
		ACEnemy* temp = Cast<ACEnemy>(i);
		temp->Show();
		temp->GetMesh()->bRenderCustomDepth = 1;
	}
	ThermalTimeline.PlayFromStart();
}

void ACPlayer::OffThermal()
{
	PostProcess->Settings.WeightedBlendables.Array[1].Weight = 0.0f;
	TArray<AActor*> arr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACEnemy::StaticClass(), arr);
	for (AActor* i : arr)
	{
		ACEnemy* temp = Cast<ACEnemy>(i);
		temp->GetMesh()->bRenderCustomDepth = 0;
	}
}

void ACPlayer::BeginOnWall()
{
	CheckFalse(GetCharacterMovement()->IsFalling());
	FVector start, end; TArray<AActor*> arr;
	FHitResult FResult,UResult;
	start = GetActorLocation();
	end = GetActorForwardVector() * 200.0f;
	bool FCheck =
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start,
		start + end,
		ETraceTypeQuery::TraceTypeQuery1,
		0,
		arr,
		EDrawDebugTrace::None,
		FResult,
		1
	);
	start += GetActorUpVector() * 20.0f;
	bool UCheck =
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start,
		start + end,
		ETraceTypeQuery::TraceTypeQuery1,
		0,
		arr,
		EDrawDebugTrace::None,
		UResult,
		1
	);
	if (!UCheck || !FCheck)return;
	FVector loc; FRotator rot;
	loc = FResult.Location + FResult.Normal * (GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	rot = UKismetMathLibrary::MakeRotFromXZ(UKismetMathLibrary::Normal(UResult.Location - FResult.Location), FResult.Normal);
	SetActorLocationAndRotation(loc, rot);
	OnWall = true;
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying, 0);
}

void ACPlayer::StopOnWall()
{
	OnWall = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling, 0);
	GetCharacterMovement()->GravityScale = 1.0f;
	SetActorRotation(FRotator());
}

void ACPlayer::Begin_Roll()
{
	FRotator r=GetActorRotation();
	r.Yaw = GetBaseAimRotation().Yaw;
	SetActorRotation(r);
	float a = GetInputAxisValue("MoveForward");
	float b = GetInputAxisValue("MoveRight");
	int32 idx = 0;
	if (FMath::IsNearlyZero(a) &&	!FMath::IsNearlyZero(b))
	{
		if (b < 0.0f) idx = 2;//좌
		else idx = 3;//우
	}
	else
	{
		if (a < 0.0f) idx = 1;//후
	}

	Montages->PlayRoll(idx);
}

void ACPlayer::End_Roll()
{
	if (Action->IsUnarmedMode() == false)
	{
		CLog::Log("End_Roll");
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	State->SetIdleMode();
}

void ACPlayer::OnHandGun()
{
	CheckFalse(State->IsIdleMode());

	Action->SetHandGunMode();
}

void ACPlayer::OnReload()
{
	Action->Reload();
}

void ACPlayer::OnSkill_1()
{
	CheckFalse(State->IsIdleMode());
	Action->DoSkill(0);
}

void ACPlayer::OnSkill_2()
{
	CheckFalse(State->IsIdleMode());
	Action->DoSkill(1);
}

void ACPlayer::OnDoAction()
{
	Action->DoAction();
}

void ACPlayer::OnAim()
{
	Action->DoOnAim();
}

void ACPlayer::OffAim()
{
	Action->DoOffAim();
}

void ACPlayer::OnSelect()
{
	CheckFalse(Action->CanSelect());
	Select = 1;
	GetController<APlayerController>()->bShowMouseCursor = 1;
	GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());
	Action->OnSelectMode();
}

void ACPlayer::OffSelect()
{
	CheckFalse(Select);
	Select = 0;
	GetController<APlayerController>()->bShowMouseCursor = 0;
	GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());
	Action->OffSelectMode();
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;
	Causer = DamageCauser;

	Action->AbortByDamaged();

	Status->SubHealth(damage);

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0.0f;
	}

	State->SetHittedMode();

	return Status->GetHealth();
}

void ACPlayer::Hitted()
{
	//PostProcess->Settings.bOverride_BloomDirtMask = true;
	//PostProcess->Settings.bOverride_BloomDirtMaskIntensity = true;
	//UKismetSystemLibrary::K2_SetTimer(this, "Hitted_End", 0.2f, false);

	Status->SetMove();

	int32 add;
	FVector f = GetActorForwardVector();
	FVector r = GetActorRightVector();
	FVector temp = Causer->GetActorForwardVector();
	f.Z = 0.0f; f.Normalize();
	r.Z = 0.0f; r.Normalize();
	temp.Z = 0.0f; temp.Normalize();
	float dot = UKismetMathLibrary::DotProduct2D(FVector2D(f.X, f.Y), FVector2D(temp.X, temp.Y));
	float fcross = UKismetMathLibrary::Cross_VectorVector(f, temp).Z;
	float rcross = UKismetMathLibrary::Cross_VectorVector(r, temp).Z;

	if (abs(dot) < FMath::Cos(45.0f))//좌우측 45~135
	{
		if (fcross > 0.0f)	add = 2;//좌측에서 피격
		else add = 3;//우측에서 피격
	}
	else
	{
		if (rcross < 0.0f)	add = 1;//후에서 피격
		else add = 0;//전방에서 피격
	}
	Montages->PlayHitted(add);
}

void ACPlayer::Hitted_End()
{
	//PostProcess->Settings.bOverride_BloomDirtMask = false;
	//PostProcess->Settings.bOverride_BloomDirtMaskIntensity = false;
}

void ACPlayer::Dead()
{
	Action->Dead();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	int32 idx = 0;

	Montages->PlayDead(idx);
}

void ACPlayer::End_Dead()
{
	Action->End_Dead();

	UKismetSystemLibrary::QuitGame(GetWorld(), GetController<APlayerController>(), EQuitPreference::Quit, false);
}

void ACPlayer::Emotion()
{
	State->SetEmotionMode();
	Status->SetStop();
	Montages->PlayEmotion();
}

void ACPlayer::End_Emotion()
{
	State->SetIdleMode();
	Status->SetMove();
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Roll:		Begin_Roll(); break;
	case EStateType::Hitted:	Hitted(); break;
	case EStateType::Dead:		Dead(); break;
	}
}

void ACPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	CheckNull(OtherComp);
	CheckTrue(OtherActor == this);

	//Cast<ACInteractDoor>(OtherActor)
	//OtherActor->IsA<ACInteractDoor>
	//if (OtherActor->GetClass()->IsChildOf(ACInteractDoor::StaticClass()))
	//	InteractDoor = Cast<ACInteractDoor>(OtherActor);
}

void ACPlayer::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);
	CheckNull(OtherComp);
	CheckTrue(OtherActor == this);

	//Cast<ACInteractDoor>(OtherActor)
	//OtherActor->IsA<ACInteractDoor>
	//if (OtherActor->GetClass()->IsChildOf(ACInteractDoor::StaticClass()))
	//	InteractDoor = nullptr;
}

void ACPlayer::StartCurse()
{
	CurseCount = 3;
	PostProcess->Settings.WeightedBlendables.Array[0].Weight = 0.01f;
	StartCurseTimeline.PlayFromStart();
}

void ACPlayer::ReduceCurse()
{
	--CurseCount;
	if (!CurseCount)
	{
		FOnTimelineEvent f;
		f.BindUFunction(this, "EndCurse");
		EndCurseTimeline.SetTimelineFinishedFunc(f);
		EndCurseTimeline.PlayFromStart();
	}
	else
	{
		ReduceCurseTimeline.PlayFromStart();
	}
}

void ACPlayer::EndCurse()
{
	PostProcess->Settings.WeightedBlendables.Array[0].Weight = 0.0f;
}

void ACPlayer::StartingCurse(float Value)
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), CurseMPC, CurseParameterName, Value);
}

void ACPlayer::ReducingCurse(float Value)
{	
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), CurseMPC, CurseParameterName,((2 - CurseCount) * 0.4) + Value);
}

void ACPlayer::EndingCurse(float Value)
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), CurseMPC, CurseParameterName,((2 - CurseCount) * 0.4) + Value);
	float temp = (100.0f-Value)/100.0f;
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), CurseMPC, FName("Weight"), temp);

}

void ACPlayer::StartingThermal(float Value)
{
	Cast<UMaterialInstanceDynamic>(PostProcess->Settings.WeightedBlendables.Array[1].Object)->SetScalarParameterValue(ThermalParameterName, Value);
}

void ACPlayer::SetInteractObject(IIInteract* InObject)
{
	InteractObject = InObject;
}

void ACPlayer::ReleaseInteractObject(IIInteract* InObject)
{
	if (InteractObject != InObject)return;
	InteractObject = nullptr;
}

#include "BehaviorTree/CBTTaskNode_SpawnPortal.h"
#include "Global.h"
#include "Objects/CPortal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy.h"
#include "Characters/CPlayer.h"
#include "Characters/CAnimInstance.h" 
#include "Components/CStatusComponent.h" 
#include "Components/CBehaviorComponent.h" 
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/AIAsyncTaskBlueprintProxy.h"

UCBTTaskNode_SpawnPortal::UCBTTaskNode_SpawnPortal()
{
	CHelpers::GetClass<ACPortal>(&PortalClass, "Blueprint'/Game/Object/Portal/BP_CPortal.BP_CPortal_C'");

	NodeName = "SpawnPortal";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_SpawnPortal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* aiPawn = Cast<ACEnemy>(controller->GetPawn());

	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	
	FVector loc = aiPawn->GetActorLocation()+ UKismetMathLibrary::Normal(aiPawn->GetActorForwardVector())*100.0f;
	FRotator rot = aiPawn->GetControlRotation();
	rot.Yaw += 180.0f;
	FTransform transform;
	transform.SetLocation(loc);

	FHitResult arr;
	FCollisionObjectQueryParams cqp;
	cqp.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	A = GetWorld()->SpawnActorDeferred<ACPortal>(PortalClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	A->SetIsPortalA(1);
	A->SetActorRotation(rot);
	if (GetWorld()->LineTraceSingleByObjectType(arr, loc, loc - UKismetMathLibrary::Normal(A->GetActorUpVector()) * 10000.0f, cqp))
	{
		A->SetActorLocation
		(
			arr.Location + 
			UKismetMathLibrary::Normal(A->GetActorUpVector()) * 124.5f// - A->GetActorForwardVector()*50.0f
		);
	}
	UGameplayStatics::FinishSpawningActor(A, transform);

	loc = behavior->GetWarpLocation();
	rot = UKismetMathLibrary::MakeRotFromX(behavior->GetTargetPlayer()->GetActorLocation() - loc);
	transform.SetLocation(loc);

	B = GetWorld()->SpawnActorDeferred<ACPortal>(PortalClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	B->SetIsPortalA(0);
	B->SetActorRotation(rot);
	if (GetWorld()->LineTraceSingleByObjectType(arr, loc, loc - UKismetMathLibrary::Normal(B->GetActorUpVector()) * 124.5f, cqp))
	{
		B->SetActorLocation(arr.Location + UKismetMathLibrary::Normal(B->GetActorUpVector()) * 124.5f);
	}
	UGameplayStatics::FinishSpawningActor(B, transform);

	A->LinkPortal(B);
	B->LinkPortal(A);

	A->OnActorEndOverlap.AddDynamic(this, &UCBTTaskNode_SpawnPortal::PortalEndOverlap);

	Flag = 0;
	Speed = 0.0f;
	Status = CHelpers::GetComponent<UCStatusComponent>(controller->GetPawn());
	Movement = CHelpers::GetComponent<UCharacterMovementComponent>(controller->GetPawn());
	Anim = Cast<UCAnimInstance>(controller->GetCharacter()->GetMesh()->GetAnimInstance());
	Anim->SetAIAccelerating(1);

	UAIAsyncTaskBlueprintProxy* f = UAIBlueprintHelperLibrary::CreateMoveToProxyObject(controller->GetWorld(), controller->GetPawn(),A->GetActorLocation(),A,0.0f);
	f->OnSuccess.AddDynamic(this, &UCBTTaskNode_SpawnPortal::Test1);
	f->OnFail.AddDynamic(this, &UCBTTaskNode_SpawnPortal::Test2);
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_SpawnPortal::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	Speed += DeltaSeconds * Status->GetRunSpeed();
	Speed = UKismetMathLibrary::FClamp(Speed, 0.0f, Status->GetRunSpeed());
	Movement->MaxWalkSpeed = Speed;

	if (Flag)
	{
		Anim->SetAIAccelerating(0);
		Movement->MaxWalkSpeed = 0.0f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UCBTTaskNode_SpawnPortal::PortalEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	FTimerHandle WaitHandle;
	float WaitTime=3.0f; 
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		if (A)A->Destroy();
		if (B)B->Destroy();
	}), WaitTime, false); 
	Flag = 1;
}

void UCBTTaskNode_SpawnPortal::Test1(EPathFollowingResult::Type MovementResult)
{
	CLog::Print("Suc",888);
}

void UCBTTaskNode_SpawnPortal::Test2(EPathFollowingResult::Type MovementResult)
{
	CLog::Print("fail", 888);
}

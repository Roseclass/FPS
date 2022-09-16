#include "BehaviorTree/CBTTaskNode_AirStrike.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy.h" 
#include "Components/CStateComponent.h" 
#include "Actions/CProjectile_AirStrike.h" 

UCBTTaskNode_AirStrike::UCBTTaskNode_AirStrike()
{
	NodeName = "AirStrike";
	CHelpers::GetClass<ACProjectile>(&ProjectileClass, "Blueprint'/Game/Enemies/Drongo/Projectiles/BP_CProjectile_AirStrike.BP_CProjectile_AirStrike_C'");
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_AirStrike::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* aiPawn = Cast<ACEnemy>(controller->GetPawn());
	State = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	FVector loc = FVector(0.0f,0.0f,2000.0f);
	FRotator rot;
	FTransform transform;
	transform.AddToTranslation(loc);
	transform.SetRotation(FQuat(rot));
	Projectile = GetWorld()->SpawnActorDeferred<ACProjectile_AirStrike>(ProjectileClass, transform, aiPawn, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	UGameplayStatics::FinishSpawningActor(Projectile, transform);

	if(State)State->SetEmotionMode();

	return EBTNodeResult::Succeeded;
	//return EBTNodeResult::InProgress;
}

void UCBTTaskNode_AirStrike::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	//if (Projectile->IsFinish())
	//{
	//	CLog::Print("Finish", 54);
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//}
	//CLog::Print("Tick", 55);
}
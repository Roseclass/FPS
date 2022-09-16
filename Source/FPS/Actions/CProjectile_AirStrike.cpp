#include "Actions/CProjectile_AirStrike.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CapsuleComponent.h"

ACProjectile_AirStrike::ACProjectile_AirStrike()
{
	InitialLifeSpan = 50.0f;
}

void ACProjectile_AirStrike::BeginPlay()
{
	Super::BeginPlay();
	for (int32 i = 1; i < Count; i++)
	{
		FTimerHandle WaitHandle;
		float WaitTime; 
		WaitTime = i * Term;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			SpawnChild();
		}), WaitTime, false);
	}
	FTimerHandle WaitHandle;
	float WaitTime;
	WaitTime = Count * Term;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		SpawnChild();
	}), WaitTime, false);
	time = 0.0f;
}

void ACProjectile_AirStrike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//time += DeltaTime;
	//if (time >= Term)
	//{
	//	SpawnChild();
	//	time = 0.0f;
	//}
}

void ACProjectile_AirStrike::SpawnChild()
{
	TArray<AActor*> temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPlayer::StaticClass(), temp);
	FVector loc = FVector::Zero();
	FRotator rotator = FRotator::ZeroRotator;
	FTransform transform;
	if (temp.Num())
	{
		FHitResult result;
		FCollisionObjectQueryParams cqp;
		cqp.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		GetWorld()->LineTraceSingleByObjectType(result, temp[0]->GetActorLocation(), temp[0]->GetActorLocation() - UKismetMathLibrary::Normal(temp[0]->GetActorUpVector()) * 10000.0f, cqp);
		loc = result.Location;
	}
	transform.AddToTranslation(loc);
	transform.SetRotation(FQuat4d(rotator));
	AActor* decal = GetWorld()->SpawnActor(DecalClass, &transform);
	decal->SetLifeSpan(3.0f);
	transform.AddToTranslation(FVector(0.0f,0.0f,1000.0f));
	AActor* bomb = GetWorld()->SpawnActorDeferred<AActor>(ChildClass, transform, GetOwner(),nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	UGameplayStatics::FinishSpawningActor(bomb, transform);
	SpawnCount++;
	if (SpawnCount == Count)
	{
		Finish = 1;
		SetLifeSpan(2.0f);
	}
}

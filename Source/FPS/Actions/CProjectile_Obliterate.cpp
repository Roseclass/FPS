#include "Actions/CProjectile_Obliterate.h"
#include "Global.h"
#include "Characters/CEnemy.h"

ACProjectile_Obliterate::ACProjectile_Obliterate()
{

}

void ACProjectile_Obliterate::BeginPlay()
{
	Super::BeginPlay();
}

void ACProjectile_Obliterate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACProjectile_Obliterate::SpawnChild()
{
	TArray<AActor*> temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyClass, temp);

	FVector loc = GetActorLocation();
	
	for (int32 i = 0; i < 10; i++)
	{
		FRotator rotator = Owner->GetCharacter()->GetActorRotation() + FRotator(FMath::RandRange(45	, 80), FMath::RandRange(-45, 45),0.0 );
		FTransform transform;
		transform.AddToTranslation(loc);
		transform.SetRotation(FQuat(rotator));
	
		ACProjectile* projectileObject = GetWorld()->SpawnActorDeferred<ACProjectile>(ChildClass, transform, Owner->GetCharacter(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (temp.Num())
		{
			ACEnemy* target = Cast<ACEnemy>(temp[i % temp.Num()]);
			projectileObject->SettingTarget(target);
		}
		UGameplayStatics::FinishSpawningActor(projectileObject, transform);
	}
}

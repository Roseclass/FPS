#include "Actions/CProjectile_Reckoning.h"
#include "Global.h"
#include "Characters/CEnemy.h"
#include "GenericTeamAgentInterface.h"

ACProjectile_Reckoning::ACProjectile_Reckoning()
{
	InitialLifeSpan = 10.0f;
}

void ACProjectile_Reckoning::BeginPlay()
{
	Super::BeginPlay();
}

void ACProjectile_Reckoning::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Time += DeltaTime;
	if (Time < TickTime)return;
	Time = 0.0f;
	FDamageEvent e;
	for (auto i : Enemies)
	{
		if (!i.Value)continue;
		i.Key->TakeDamage(Power, e, Owner, this);
	}
}

void ACProjectile_Reckoning::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == GetOwner());
	if (Cast<ACProjectile>(OtherActor))return;
	IGenericTeamAgentInterface* own = Cast<IGenericTeamAgentInterface>(GetOwner());
	IGenericTeamAgentInterface* other = Cast<IGenericTeamAgentInterface>(OtherActor);
	if (own && other)
	{
		if (own->GetGenericTeamId().GetId() == other->GetGenericTeamId().GetId())return;
	}

	ACEnemy* enemy = Cast<ACEnemy>(other);
	CheckNull(enemy);
	Enemies.Add(enemy, 1);
	FDamageEvent e;
	enemy->TakeDamage(Power, e, Owner, this);
}

void ACProjectile_Reckoning::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACEnemy* other = Cast<ACEnemy>(OtherActor);
	CheckNull(other);
	if(Enemies.Contains(other))Enemies[other] = 0;
}

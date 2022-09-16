#include "Actions/CProjectile_Bomb.h"
#include "Global.h"
#include "GenericTeamAgentInterface.h"

ACProjectile_Bomb::ACProjectile_Bomb()
{
	InitialLifeSpan = 300.0f;
}

void ACProjectile_Bomb::BeginPlay()
{
	Super::BeginPlay();
	OverlapFloat.BindUFunction(this, "SetSphereRadius");
	OverlapTimeline.AddInterpFloat(OverlapCurve, OverlapFloat);
	FOnTimelineEvent eve;
	eve.BindUFunction(this, "EndFunction");
	OverlapTimeline.SetTimelineFinishedFunc(eve);
}

void ACProjectile_Bomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OverlapTimeline.TickTimeline(DeltaTime);
}

void ACProjectile_Bomb::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bActive)
	{
		DoExplosion();
		bActive = 1;
		OverlapTimeline.PlayFromStart();
	}
	CheckTrue(OtherActor == GetOwner());
	if (Cast<ACProjectile>(OtherActor))return;
	IGenericTeamAgentInterface* own = Cast<IGenericTeamAgentInterface>(GetOwner());
	IGenericTeamAgentInterface* other = Cast<IGenericTeamAgentInterface>(OtherActor);
	if (own && other)
	{
		if (own->GetGenericTeamId().GetId() == other->GetGenericTeamId().GetId())return;
	}
	CheckTrue(Map.Contains(OtherActor));

	FDamageEvent e;
	OtherActor->TakeDamage(Power, e, Owner, this);
	Map.Add(OtherActor, 1);
}

void ACProjectile_Bomb::EndFunction()
{
	CLog::Print("End");
	Destroy();
}

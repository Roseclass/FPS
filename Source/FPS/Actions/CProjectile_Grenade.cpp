#include "Actions/CProjectile_Grenade.h"
#include "Global.h"
#include "GenericTeamAgentInterface.h"

ACProjectile_Grenade::ACProjectile_Grenade()
{
	InitialLifeSpan = 300.0f;
}

void ACProjectile_Grenade::BeginPlay()
{
	Super::BeginPlay();
	OverlapFloat.BindUFunction(this, "SetSphereRadius");
	OverlapTimeline.AddInterpFloat(OverlapCurve, OverlapFloat);
	FOnTimelineEvent eve;
	eve.BindUFunction(this, "EndFunction");
	OverlapTimeline.SetTimelineFinishedFunc(eve);
	SetSphereRadius(0.0f);
	FTimerHandle WaitHandle;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		bActive = 1;
		OverlapTimeline.PlayFromStart();
		DoExplosion();
	}), CountTime, false);
}

void ACProjectile_Grenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OverlapTimeline.TickTimeline(DeltaTime);
}

void ACProjectile_Grenade::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckFalse(bActive);
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

void ACProjectile_Grenade::EndFunction()
{
	Destroy();
}

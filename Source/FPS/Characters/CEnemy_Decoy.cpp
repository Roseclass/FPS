#include "Characters/CEnemy_Decoy.h"
#include "Global.h"
#include "Kismet/KismetMaterialLibrary.h"

ACEnemy_Decoy::ACEnemy_Decoy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->CastShadow = 0;
}

void ACEnemy_Decoy::BeginPlay()
{
	Super::BeginPlay();
	
	PiercingFloat.BindUFunction(this, "Piercing");
	PiercingTimeline.AddInterpFloat(PiercingCurve, PiercingFloat);

}

void ACEnemy_Decoy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PiercingTimeline.TickTimeline(DeltaTime);
}

void ACEnemy_Decoy::Piercing(float Value)
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), PiercingMPC, PiercingRadiusName, Value);
}

void ACEnemy_Decoy::DoPierce(FVector InLocation)
{
	CLog::Print("IN");
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), PiercingMPC, PiercingLocationName, FLinearColor(InLocation));
	PiercingTimeline.PlayFromStart();
}

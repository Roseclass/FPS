#include "Actions/CProjectile_Path.h"
#include "Global.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACProjectile_Path::ACProjectile_Path()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this,&Spline, "Spline");

}

void ACProjectile_Path::BeginPlay()
{
	Super::BeginPlay();
	CheckNull(OwnerCharacter);

	//Setting prediction
	FPredictProjectilePathResult result;
	Input.StartLocation=OwnerCharacter->GetMesh()->GetSocketLocation(BoneName);
	FRotator up = UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetActorUpVector());
	FRotator forward = OwnerCharacter->GetControlRotation();
	FRotator lerp = UKismetMathLibrary::RLerp(up, forward, 0.7f, 1);
	FVector dir = UKismetMathLibrary::Conv_RotatorToVector(lerp);
	Input.LaunchVelocity =	UKismetMathLibrary::Normal(dir)* Velocity;
	UGameplayStatics::PredictProjectilePath(GetWorld(), Input, result);

	for (int32 i =0; i< result.PathData.Num(); i++)
	{
		FPredictProjectilePathPointData data = result.PathData[i];
		Spline->AddSplinePointAtIndex(data.Location,i,ESplineCoordinateSpace::World);
	}	
	Spline->SetSplinePointType(result.PathData.Num() - 1, ESplinePointType::CurveClamped);
	for (int32 i = 0; i < result.PathData.Num() - 1; i++)//컴포넌트 추가 부분에서 문제가있는듯? 중간에 프레임이 끊김 블프에선 잘돌아감;;
	{
		//USplineMeshComponent* temp = Cast<USplineMeshComponent>(AddComponentByClass(USplineMeshComponent::StaticClass(), 1, FTransform(), 1));
		//temp->SetMobility(EComponentMobility::Movable);
		//temp->SetForwardAxis(ESplineMeshAxis::Z);
		//temp->SetStaticMesh(Mesh);
		//Mesh->SetMaterial(0, Materials[0]);		
		//FinishAddComponent(temp, 1, FTransform());
		//temp->SetStartScale(FVector2D(0.1f));
		//temp->SetEndScale(FVector2D(0.1f));
		FVector loc, tan, nextloc, nexttan;
		Spline->GetLocationAndTangentAtSplinePoint(i, loc, tan, ESplineCoordinateSpace::World);
		Spline->GetLocationAndTangentAtSplinePoint(i+1, nextloc, nexttan, ESplineCoordinateSpace::World);
		//temp->SetStartAndEnd(loc, tan, nextloc, nexttan);
		AddSplineMesh(loc, tan, nextloc, nexttan);
	}
}

void ACProjectile_Path::SetInputVelocity(float InVelocity)
{
	Velocity = InVelocity;
}

void ACProjectile_Path::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACProjectile_Path::SetData(ACharacter* InCharacter, AActor* InProjectile)
{
	OwnerCharacter = InCharacter;
	OwnerProjectile = InProjectile;
}

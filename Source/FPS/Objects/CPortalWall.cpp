#include "Objects/CPortalWall.h"
#include "Global.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Objects/CPortal.h"

ACPortalWall::ACPortalWall()
{
	CHelpers::CreateComponent(this, &Root, "Root");
	CHelpers::CreateComponent(this, &Mesh, "Mesh", Root);

	Mesh->SetGenerateOverlapEvents(1);
}

void ACPortalWall::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Mesh->SetWorldScale3D(FVector(1.0f, WallWidth / 100.0f, WallHeight / 100.0f));
}

void ACPortalWall::BeginPlay()
{
	Super::BeginPlay();	
}

FVector2D ACPortalWall::ConstrainPortalToWall(float PortalY, float PortalZ)
{
	float clampedY = ClampPointToWall(PortalY, WallWidth, 180.0f / 2.0f);
	float clampedZ = ClampPointToWall(PortalZ, WallHeight, 249.0f / 2.0f);
	return FVector2D(clampedY, clampedZ);
}

float ACPortalWall::ClampPointToWall(float Point, float WallSize, float PortalRadius)
{
	float offset;
	offset = (WallSize / 2) - PortalRadius - abs(Point);
	offset = FMath::Clamp(offset, -10000000, 0);
	Point < 0.0f ? offset *= 1.0f: offset *= -1.0f;
	return Point - offset;
}

bool ACPortalWall::RectToRectCollision(FVector2D AOrigin, FVector2D AExtense, FVector2D BOrigin, FVector2D BExtense)
{
	if (AOrigin.X + AExtense.X < BOrigin.X - BExtense.X)return 0;
	if (AOrigin.X - AExtense.X > BOrigin.X + BExtense.X)return 0;
	if (AOrigin.Y + AExtense.Y < BOrigin.Y - BExtense.Y)return 0;
	if (AOrigin.Y - AExtense.Y > BOrigin.Y + BExtense.Y)return 0;
	return 1;
}

ACPortal* ACPortalWall::TryAddPortal(FVector PortalOrigin, bool IsPortalA)
{
	FVector relative;
	relative = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), PortalOrigin);
	FVector2D result = ConstrainPortalToWall(relative.Y, relative.Z);
	relative.Y = result.X;relative.Z = result.Y;
	if (HasRoomForNewPortal(relative.Y, relative.Z))
	{
		FVector loc;
		loc = UKismetMathLibrary::TransformLocation(GetActorTransform(), relative);
		FTransform transform;
		transform.SetTranslation(loc);
		transform.SetRotation(FQuat(GetActorRotation()));
		ACPortal* portal = GetWorld()->SpawnActorDeferred<ACPortal>(PortalClass, transform,nullptr,nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		portal->SetIsPortalA(IsPortalA);
		UGameplayStatics::FinishSpawningActor(portal, transform);
		PortalsOnWall.Add(portal);
		portal->OnDestroyed.AddDynamic(this,&ACPortalWall::OnPortalDestroyed);
		return portal;
	}
	return nullptr;
}

void ACPortalWall::OnPortalDestroyed(AActor* DestroyedActor)
{
	PortalsOnWall.Remove(DestroyedActor);
}

bool ACPortalWall::HasRoomForNewPortal(float NewPortalY, float NewPortalZ)
{
	for (AActor* i : PortalsOnWall)
	{
		FVector vec = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), i->GetActorLocation());
		if
			(
				RectToRectCollision(
					FVector2D(vec.Y, vec.Z), FVector2D(180.0f / 2.0f, 249.0f / 2.0f),
					FVector2D(NewPortalY, NewPortalZ), FVector2D(180.0f / 2.0f, 249.0f / 2.0f)
				))	return 0;
	}
	return 1;
}
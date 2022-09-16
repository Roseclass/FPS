#include "Objects/CPortal.h"
#include "Global.h"
#include "Objects/CPortalWall.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/CharacterMovementComponent.h"

ACPortal::ACPortal()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Root, "Root");
	CHelpers::CreateComponent(this, &PortalBorderMesh, "PortalBorderMesh", Root);
	CHelpers::CreateComponent(this, &PortalMesh,"PortalMesh", Root);
	CHelpers::CreateComponent(this, &SceneCapture2D,"SceneCapture2D", Root);
	CHelpers::CreateComponent(this, &BackFacingScene,"BackFacingScene", Root);

	PortalBorderMesh->CastShadow = 0;
	SceneCapture2D->bOverride_CustomNearClippingPlane = 1;
	SceneCapture2D->CustomNearClippingPlane = 200.0f;
}

void ACPortal::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	PortalA ? PortalBorderMesh->SetMaterial(0, BorderMaterialA) : PortalBorderMesh->SetMaterial(0, BorderMaterialB);
	PortalA ? SceneCapture2D->TextureTarget = TextureB : SceneCapture2D->TextureTarget = TextureA;

}

void ACPortal::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ACPortal::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACPortal::ActorEndOverlap);
	OnDestroyed.AddDynamic(this, &ACPortal::ActorDestroyed);
}

void ACPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FTransform transform =
	//	UKismetMathLibrary::ConvertTransformToRelative(
	//		BackFacingScene->GetComponentTransform(),
	//		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentTransform()
	//	);
	//float clipping = 1.0f +
	//	UKismetMathLibrary::Vector_Distance(
	//		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentLocation(),
	//		GetActorLocation()
	//	);
	CheckNull(LinkedPortal);
	//LinkedPortal->TickFunction(transform.GetLocation(), FRotator(transform.GetRotation()), clipping);
	for (ACharacter* i : ActorsInPortal)CheckIfActorShouldTeleport(i);
}

void ACPortal::TeleportActor(ACharacter* Target)
{
	FVector relativeVelocity =
	UKismetMathLibrary::InverseTransformDirection(Target->GetActorTransform(), Target->GetVelocity());
	FVector loc; FRotator rot; FTransform transform;
	transform = UKismetMathLibrary::ConvertTransformToRelative(BackFacingScene->GetComponentTransform(), Target->GetActorTransform());
	transform = UKismetMathLibrary::ComposeTransforms(transform, LinkedPortal->GetActorTransform());
	loc = transform.GetLocation();
	loc += CHelpers::GetComponent<USkeletalMeshComponent>(Target)->GetRelativeLocation();
	loc += LinkedPortal->GetActorForwardVector() * 10.0f;
	Target->SetActorLocation(loc,0,nullptr,ETeleportType::TeleportPhysics);
	rot = FRotator(transform.GetRotation());
	rot.Roll = 0.0f;
	Target->GetController()->SetControlRotation(rot);
	FTransform t;
	t.SetTranslation(Target->GetActorLocation());
	t.SetRotation(FQuat4d(Target->GetController()->GetControlRotation()));
	Target->GetMovementComponent()->Velocity=UKismetMathLibrary::TransformDirection(t, relativeVelocity);
	UseCount--;
	if (!UseCount)
	{
		if(LinkedPortal)LinkedPortal->Destroy();
		Destroy();
	}
}

void ACPortal::LinkPortal(ACPortal* InPortal)
{
	LinkedPortal = InPortal;
	if (LinkedPortal)	PortalA ? PortalMesh->SetMaterial(0, MeshMaterialA) : PortalMesh->SetMaterial(0, MeshMaterialB);
	else PortalMesh->SetMaterial(0, MeshMaterialNull);
}

void ACPortal::CheckIfActorShouldTeleport(ACharacter* Target)
{
	FVector vec;
	vec = Target->GetActorLocation() + Target->GetVelocity() * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())-GetActorLocation();
	vec = UKismetMathLibrary::Normal(vec);
	if(FVector::DotProduct(vec, GetActorForwardVector())<=0.0f)TeleportActor(Target);
}

void ACPortal::SetIsPortalA(bool IsPortalA)
{
	PortalA = IsPortalA;
}

void ACPortal::TickFunction(FVector InLocation, FRotator InRotation, float InCustomClipping)
{
	SceneCapture2D->SetRelativeLocationAndRotation(InLocation, InRotation);
	SceneCapture2D->CustomNearClippingPlane=InCustomClipping;
}

void ACPortal::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	CheckNull(LinkedPortal);
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);
	int32 idx = ActorsInPortal.AddUnique(character);
	if (idx == -1)return;
	UCapsuleComponent* capsule = CHelpers::GetComponent<UCapsuleComponent>(OtherActor);
	CheckNull(capsule);
	capsule->SetCollisionProfileName("PortalPawn");
}

void ACPortal::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);
	ActorsInPortal.Remove(character);
	UCapsuleComponent* capsule = CHelpers::GetComponent<UCapsuleComponent>(OtherActor);
	CheckNull(capsule);
	capsule->SetCollisionProfileName("Pawn");
}

void ACPortal::SetUseCount(int32 InCount)
{
	UseCount = InCount;
}

void ACPortal::ActorDestroyed(AActor* DestroyedActor)
{
	for (ACharacter* character : ActorsInPortal)
	{
		UCapsuleComponent* capsule = CHelpers::GetComponent<UCapsuleComponent>(character);
		if (capsule == nullptr) continue;
		capsule->SetCollisionProfileName("Pawn");
	}
}

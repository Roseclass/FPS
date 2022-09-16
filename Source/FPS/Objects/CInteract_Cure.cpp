#include "Objects/CInteract_Cure.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Interact.h"
#include "Particles/ParticleSystemComponent.h"

ACInteract_Cure::ACInteract_Cure()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create SceneComponent
	CHelpers::CreateComponent(this, &Mesh, "Mesh");
	CHelpers::CreateComponent(this, &Box, "Box", Mesh);
	CHelpers::CreateComponent(this, &Widget, "Widget", Mesh);

	Widget->SetRelativeLocation(FVector(0, 0, 120));
	Widget->SetDrawSize(FVector2D(48, 48));
	Widget->SetWidgetSpace(EWidgetSpace::Screen);

	Mesh->SetRenderCustomDepth(true);
	Mesh->SetCustomDepthStencilValue(1.0f);

}

void ACInteract_Cure::BeginPlay()
{
	Super::BeginPlay();	
	Widget->InitWidget();
	ConcealWidget();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACInteract_Cure::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACInteract_Cure::OnComponentEndOverlap);

	UCUserWidget_Interact* temp = Cast<UCUserWidget_Interact>(Widget->GetUserWidgetObject());
	temp->OnInteractWidgetDelegate.AddDynamic(this, &ACInteract_Cure::InteractComplete);
}

void ACInteract_Cure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACInteract_Cure::RevealWidget()
{
	Widget->SetVisibility(1);
}

void ACInteract_Cure::ConcealWidget()
{
	Widget->SetVisibility(0);
}

void ACInteract_Cure::Interact()
{
	CheckFalse(bActive);
	if (OnStartDelegate.IsBound())
		OnStartDelegate.Broadcast();
}

void ACInteract_Cure::Interact_End()
{
	CheckFalse(bActive);
	UCUserWidget_Interact* temp = Cast<UCUserWidget_Interact>(Widget->GetUserWidgetObject());

	if (!temp->IsSucceed())InteractCease();
}

void ACInteract_Cure::InteractCease()
{
	CheckFalse(bActive);
	if (OnCeaseDelegate.IsBound())
		OnCeaseDelegate.Broadcast();
}

void ACInteract_Cure::InteractComplete()
{
	CheckFalse(bActive);
	bActive = 0;
	Mesh->SetRenderCustomDepth(0);
	PlayParticleEffect();
	if (OnCompleteDelegate.IsBound())
		OnCompleteDelegate.Broadcast();
}

void ACInteract_Cure::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckFalse(bActive);
	ACPlayer* character = Cast<ACPlayer>(OtherActor);
	CheckNull(character);
	RevealWidget();

	character->SetInteractObject(Cast<IIInteract>(this));

	OnCompleteDelegate.AddDynamic(character, &ACPlayer::ReduceCurse);

	UCUserWidget_Interact* temp = Cast<UCUserWidget_Interact>(Widget->GetUserWidgetObject());
	OnStartDelegate.AddDynamic(temp, &UCUserWidget_Interact::StartInteraction);
	OnCeaseDelegate.AddDynamic(temp, &UCUserWidget_Interact::CeaseInteraction);
}

void ACInteract_Cure::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPlayer* character = Cast<ACPlayer>(OtherActor);
	CheckNull(character);
	ConcealWidget();

	character->ReleaseInteractObject(Cast<IIInteract>(this));

	OnStartDelegate.Clear(); OnCeaseDelegate.Clear(); OnCompleteDelegate.Clear();
}

void ACInteract_Cure::PlayParticleEffect()
{
	if (!Effect) return;
	FTransform transform;
	transform.SetTranslation(GetActorLocation());
	transform.SetRotation(FQuat(GetActorRotation()));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, transform);
}

void ACInteract_Cure::SetActive()
{
	bActive = 1;
}

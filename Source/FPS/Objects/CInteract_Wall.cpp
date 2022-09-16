#include "Objects/CInteract_Wall.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

ACInteract_Wall::ACInteract_Wall()
{
	CHelpers::CreateComponent(this, &Mesh, "Mesh");
	CHelpers::CreateComponent(this, &Box, "Box", Mesh);

}

void ACInteract_Wall::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACInteract_Wall::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACInteract_Wall::OnComponentEndOverlap);

}

void ACInteract_Wall::Interact()
{
	if (OnStartDelegate.IsBound())
		OnStartDelegate.Broadcast();
}

void ACInteract_Wall::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* character = Cast<ACPlayer>(OtherActor);
	CheckNull(character);

	character->SetInteractObject(Cast<IIInteract>(this));
	OnStartDelegate.AddDynamic(character, &ACPlayer::BeginOnWall);

	CLog::Print("in");
}

void ACInteract_Wall::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPlayer* character = Cast<ACPlayer>(OtherActor);
	CheckNull(character);
	ConcealWidget();

	character->ReleaseInteractObject(Cast<IIInteract>(this));

	OnStartDelegate.Clear();
}
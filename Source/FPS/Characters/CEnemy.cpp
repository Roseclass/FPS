#include "Characters/CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CBoneComponent.h"
#include "Widgets/CUserWidget_DamageText.h"
#include "Widgets/CUserWidget_HealthBar.h"
#include "Actions/CActionData.h"
#include "Actions/CAction.h"
#include "Kismet/KismetMaterialLibrary.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Bone, "Bone");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;

}

void ACEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ACEnemy::BeginPlay()
{
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);
	Super::BeginPlay();

	HideFloat.BindUFunction(this,"Hiding");
	HideTimeline.AddInterpFloat(HideCurve, HideFloat);
	if (HealthBarClass)
	{
		HealthBar = CreateWidget<UCUserWidget_HealthBar, APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0), HealthBarClass);
		HealthBar->AddToViewport();
		HealthBar->SetVisibility(ESlateVisibility::HitTestInvisible);
		HealthBar->SetOwner(this);
		Status->OnAddHealth.AddDynamic(HealthBar, &UCUserWidget_HealthBar::Update);
		Status->OnSubHealth.AddDynamic(HealthBar, &UCUserWidget_HealthBar::Update);
	}
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HideTimeline.TickTimeline(DeltaTime);
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;
	Causer = DamageCauser;

	if (!SuperArmor)Action->AbortByDamaged();

	Status->SubHealth(damage);

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0.0f;
	}
	if(!SuperArmor)	State->SetHittedMode();

	FTransform transform;
	transform.AddToTranslation(GetActorLocation()+FVector(FMath::RandRange(-40,40), FMath::RandRange(-20, 20),100));

	AActor* dmg = GetWorld()->SpawnActorDeferred<AActor>(DamageTextClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	UGameplayStatics::FinishSpawningActor(dmg, transform);
	UWidgetComponent* widget = CHelpers::GetComponent<UWidgetComponent>(dmg);
	UCUserWidget_DamageText* txt =Cast<UCUserWidget_DamageText>(widget->GetUserWidgetObject());
	if (txt)
	{
		txt->SetRedColor(Bone->GetCurrentHittedBoneRate());
		txt->SetText(Damage);
	}

	return Status->GetHealth();
}

void ACEnemy::Hitted()
{
	Status->SetMove();

	int32 add;
	FVector f = GetActorForwardVector();
	FVector r = GetActorRightVector();
	FVector temp = Causer->GetActorForwardVector();
	f.Z = 0.0f; f.Normalize();
	r.Z = 0.0f; r.Normalize();
	temp.Z = 0.0f; temp.Normalize();
	float dot = UKismetMathLibrary::DotProduct2D(FVector2D(f.X,f.Y), FVector2D(temp.X, temp.Y));
	float fcross  = UKismetMathLibrary::Cross_VectorVector(f,temp).Z;
	float rcross  = UKismetMathLibrary::Cross_VectorVector(r,temp).Z;

	if (abs(dot) < FMath::Cos(45.0f))//좌우측 45~135
	{
		if (fcross > 0.0f)	add = 2;//좌측에서 피격
		else add = 3;//우측에서 피격
	}
	else
	{
		if (rcross < 0.0f)	add = 1;//후에서 피격
		else add = 0;//전방에서 피격
	}
	Montages->PlayHitted(add);	
}

void ACEnemy::Dead()
{
	CheckFalse(State->IsDeadMode());

	Action->Dead();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthBar->RemoveFromParent();
	Montages->PlayDead();
}

void ACEnemy::Emotion()
{
	int32 i = UKismetMathLibrary::RandomIntegerInRange(0, 100);

	Montages->PlayEmotion(i);
}

void ACEnemy::End_Dead()
{
	Action->End_Dead();

	Destroy();
}

void ACEnemy::End_Emotion()
{
	State->SetIdleMode();
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead:	Dead();	break;
	case EStateType::Emotion: Emotion(); break;
	}
}

FGenericTeamId ACEnemy::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACEnemy::Hiding(float Value)
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), HideMPC, HideParameterName, Value);
}

void ACEnemy::Hide()
{
	HideTimeline.PlayFromStart();
}

void ACEnemy::Hide_End()
{
	if(UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), HideMPC, HideParameterName)==0.25f)return;
	HideTimeline.ReverseFromEnd();
}

void ACEnemy::Show()
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), HideMPC, HideParameterName, 0.25f);
}

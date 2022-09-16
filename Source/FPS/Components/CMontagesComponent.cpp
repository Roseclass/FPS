#include "Components/CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMontagesComponent::UCMontagesComponent()
{

}

void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);

	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontageData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;
				continue;
			}
		}
	}
}

void UCMontagesComponent::PlayAnimMontage(EStateType InStateType, int32 InIndex)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FMontageData* data = Datas[(int32)InStateType];
	if (!!data)
	{
		if (!!data->AnimMontages.Num())
			character->PlayAnimMontage(data->AnimMontages[InIndex], data->PlayRates[InIndex], data->StartSections[InIndex]);
	}
}

void UCMontagesComponent::PlayRoll(int32 InIndex)
{
	PlayAnimMontage(EStateType::Roll, InIndex);
}

void UCMontagesComponent::PlayHitted(int32 InIndex)
{
	PlayAnimMontage(EStateType::Hitted, InIndex);
}

void UCMontagesComponent::PlayDead(int32 InIndex)
{
	PlayAnimMontage(EStateType::Dead, InIndex);
}

void UCMontagesComponent::PlayEmotion(int32 InIndex)
{
	InIndex %= Datas[(int32)EStateType::Emotion]->AnimMontages.Num();
	PlayAnimMontage(EStateType::Emotion, InIndex);
}

float UCMontagesComponent::GetPlayLength(int32 DataIndex, int32 InIndex)
{
	InIndex %= Datas[DataIndex]->AnimMontages.Num();
	return Datas[DataIndex]->AnimMontages[InIndex]->GetPlayLength();
}


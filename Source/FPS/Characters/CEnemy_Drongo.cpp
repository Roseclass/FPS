#include "Characters/CEnemy_Drongo.h"
#include "Global.h"
#include "Components/CBoneComponent.h"

ACEnemy_Drongo::ACEnemy_Drongo()
{
	//메시설정
	USkeletalMesh* mesh;
	CHelpers::GetAsset(&mesh, "SkeletalMesh'/Game/ParagonDrongo/Characters/Heroes/Drongo/Meshes/Drongo_GDC.Drongo_GDC'");
	GetMesh()->SkeletalMesh = mesh;
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	//애니메이션 설정
	TSubclassOf<UAnimInstance> anim;
	CHelpers::GetClass<UAnimInstance>(&anim, "AnimBlueprint'/Game/Enemies/Drongo/Animation/ABP_CEnemy_Drongo.ABP_CEnemy_Drongo_C'");
	GetMesh()->AnimClass = anim;
	//본컴포넌트->피직스에셋->본트리 읽어오기
	UPhysicsAsset* physics;
	CHelpers::GetAsset(&physics, "PhysicsAsset'/Game/ParagonDrongo/Characters/Heroes/Drongo/Meshes/NewPhysicsAsset.NewPhysicsAsset'");
	Bone->PhysicsAsset = physics;
	Bone->ReadBoneData();
	
}
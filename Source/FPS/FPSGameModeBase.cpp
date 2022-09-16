#include "FPSGameModeBase.h"
#include "Global.h"
#include "CHUD.h"
#include "Characters/CPlayer.h"

AFPSGameModeBase::AFPSGameModeBase()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");
	CHelpers::GetClass<AHUD>(&HUDClass, "Blueprint'/Game/Player/BP_CHUD.BP_CHUD_C'");
}

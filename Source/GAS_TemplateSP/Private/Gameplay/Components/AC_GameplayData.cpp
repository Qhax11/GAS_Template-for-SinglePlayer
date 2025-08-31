// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_GameplayData.h"


UAC_GameplayData::UAC_GameplayData()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UDA_ActorSounds* UAC_GameplayData::GetActorSoundsData()
{
	if (!IsValid(DA_ActorSounds) || !DA_ActorSounds)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActorSounds is null in: %s."), *GetName());
		return nullptr;
	}

	return DA_ActorSounds;
}

UDA_AttackTypeToCameraShake* UAC_GameplayData::GetAttackTypeToCameraShakeData()
{
	if (!IsValid(DA_AttackTypeToCameraShake) || !DA_AttackTypeToCameraShake)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackTypeToCameraShake is null in: %s."), *GetName());
		return nullptr;
	}

	return DA_AttackTypeToCameraShake;
}

UDA_AttackTypeToCameraZoom* UAC_GameplayData::GetAttackTypeToCameraZoomData()
{
	if (!IsValid(DA_AttackTypeToCameraZoom) || !DA_AttackTypeToCameraZoom)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackTypeToCameraShake is null in: %s."), *GetName());
		return nullptr;
	}

	return DA_AttackTypeToCameraZoom;
}





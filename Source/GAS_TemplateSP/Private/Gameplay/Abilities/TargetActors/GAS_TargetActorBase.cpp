// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/GAS_TargetActorBase.h"
#include "Abilities/GameplayAbility.h"

void AGAS_TargetActorBase::Confirm()
{
	UE_LOG(LogTemp, Warning, TEXT("Confirm"));
}

void AGAS_TargetActorBase::Cancel()
{
	UE_LOG(LogTemp, Warning, TEXT("Cancel"));
}

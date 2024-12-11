// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/GAS_TargetActorBase.h"
#include "Abilities/GameplayAbility.h"


void AGAS_TargetActorBase::Confirm()
{
	OnConfirm.Broadcast(FGAS_TargetActorData(this, nullptr));
}

void AGAS_TargetActorBase::Cancel()
{
	OnCancel.Broadcast(FGAS_TargetActorData(this, nullptr));
}

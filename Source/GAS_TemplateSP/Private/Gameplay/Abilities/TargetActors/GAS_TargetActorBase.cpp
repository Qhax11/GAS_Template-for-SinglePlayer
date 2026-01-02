// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/GAS_TargetActorBase.h"
#include "Abilities/GameplayAbility.h"


void AGAS_TargetActorBase::Confirm()
{
	// Logic will be implmeneted in subclasses.
}

void AGAS_TargetActorBase::Cancel()
{
	// Logic will be implmeneted in subclasses.
}

void AGAS_TargetActorBase::DestroyTargetActor()
{
	bBeingDestroyed = true;
	Destroy();
}

void AGAS_TargetActorBase::BrodcastInitalized()
{
	OnInitialized.Broadcast();
}


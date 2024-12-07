// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/TargetActors/GAS_TargetActorBase.h"
#include "Abilities/GameplayAbility.h"

void AGAS_TargetActorBase::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	PrimaryPC = Cast<APlayerController>(Ability->GetAvatarActorFromActorInfo()->GetInstigatorController());
}

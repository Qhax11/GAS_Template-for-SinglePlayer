// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/InCombat/Attack/GA_ComboMeleeAttack.h"

UGA_ComboMeleeAttack::UGA_ComboMeleeAttack()
{
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_MeleeCombo);
	SetAssetTags(AssetTags);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo);
}

void UGA_ComboMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_ComboMeleeAttack::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnEventReceived(EventTag, EventData);

	if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_Attack_CanActivateNextAttack) 
	{
		OnCanExecuteNextAttack.Broadcast();
	}
}




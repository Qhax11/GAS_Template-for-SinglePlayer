// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_DodgeBase.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UGA_DodgeBase::UGA_DodgeBase()
{
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Movement_Dash);

	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Basic);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Finisher);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Moving_Dash);
}

void UGA_DodgeBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, DodgeDamageImmunityDuration);
	if (!WaitDelayTask)
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_DamageImmune);
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
	WaitDelayTask->OnFinish.AddDynamic(this, &UGA_DodgeBase::RemoveDamageImmuneTag);
	WaitDelayTask->ReadyForActivation();
}

FVector UGA_DodgeBase::CalculateMotionWarpingLocation() const
{
	return Super::CalculateMotionWarpingLocation();
}

void UGA_DodgeBase::RemoveDamageImmuneTag()
{
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_DamageImmune, 100);
}


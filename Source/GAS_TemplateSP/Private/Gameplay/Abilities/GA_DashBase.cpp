// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_DashBase.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"

UGA_DashBase::UGA_DashBase()
{
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Movement_Dash);

	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Basic);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Finisher);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Moving_Dash);
}

void UGA_DashBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_DashBase::BindRootMotionTask()
{
	if (DashRootMotionTask)
	{
		DashRootMotionTask->OnTimedOut.AddDynamic(this, &UGA_DashBase::OnTaskTimedOut);
		DashRootMotionTask->OnTimedOutAndDestinationReached.AddDynamic(this, &UGA_DashBase::OnTimedOutAndDestinationReached);
		DashRootMotionTask->ReadyForActivation();
	}
}

FVector UGA_DashBase::CalculateDestination()
{
	return FVector();
}

void UGA_DashBase::OnTaskTimedOut()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_DashBase::OnTimedOutAndDestinationReached()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}


// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/GA_EnemyVulnerable.h"

UGA_EnemyVulnerable::UGA_EnemyVulnerable()
{
    TagsToRemove.AddTag(GAS_Tags::TAG_Gameplay_State_LockRotationTowardsTarget);
}

void UGA_EnemyVulnerable::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    // CRITICAL FIX: Check if ASC is valid before accessing it
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (ASC && IsValid(ASC))
    {
        if (ASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Vulnerable))
        {
            ASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Vulnerable, 100);
        }
    }

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

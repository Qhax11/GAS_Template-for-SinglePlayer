// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Router/GA_HeroRouterAttack.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"


void UGA_HeroRouterAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
    if (!HeroBase) 
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        return;
    }

    HeroMeleeComboManager = HeroBase->GetHeroMeleeComboManagerComponent();
    if (!HeroMeleeComboManager) 
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroMeleeComboManager is null in: %s"), *GetName());
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        return;
    }

    if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_Moving_Running))
    {
        GetASC()->TryActivateAbilityByClassAndReturnInstance(GA_RunAttackClass);
    }
    else
    {
        HeroMeleeComboManager->ActivateComboMeleeAttackAbility();
    }
   
}
// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/InCombat/Attack/GA_ComboMeleeAttack.h"
#include "GA_Hero_ShadowCombo.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_Hero_ShadowCombo : public UGA_ComboMeleeAttack
{
	GENERATED_BODY()
	
public:
	UGA_Hero_ShadowCombo();

    // ? PreActivate override et
    virtual void PreActivate(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
        const FGameplayEventData* TriggerEventData = nullptr
    ) override;

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

};

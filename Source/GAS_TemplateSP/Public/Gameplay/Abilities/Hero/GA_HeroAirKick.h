// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"
#include "GA_HeroAirKick.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroAirKick : public UGA_MeleeAttackBase
{
	GENERATED_BODY()
	
public:
	UGA_HeroAirKick();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void AttackLogic(const TArray<FHitResult>& OutHitResults) override;

	UPROPERTY(EditDefaultsOnly)
	float LaunchStrength = 600.0f;

	UPROPERTY(EditDefaultsOnly)
	float ZStrength = 3.0f;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ReverseJump;

	UPROPERTY(EditDefaultsOnly)
	bool bEnableDebug;
};

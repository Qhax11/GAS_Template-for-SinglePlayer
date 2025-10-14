// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_TracePefromerOnMontage.h"
#include "GA_MeleeAttackBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_MeleeAttackBase : public UGA_TracePefromerOnMontage
{
	GENERATED_BODY()

public:
	UGA_MeleeAttackBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

	virtual void OnTraceHitResults(const TArray<FHitResult>& HitResults) override;

	void AttackLogic(const TArray<FHitResult>& OutHitResults);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	FGameplayTag GetAttackTypeTagFromAbilityTags() const;

	/**
    * Retrieves the attack direction tag from the incoming melee attack.
    * Each melee attack ability contains its own AttackDirection tag (LeftToRight, RightToLeft, etc.).
    */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	FGameplayTag GetAttackDirectionTagFromAbilityTags() const;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameters")
	TSubclassOf<UGameplayEffect> GEPhysicalDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameters")
	FGameplayTagContainer TagsToAddToPhysicalDamageEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameters")
    FScalableFloat Damage;
};

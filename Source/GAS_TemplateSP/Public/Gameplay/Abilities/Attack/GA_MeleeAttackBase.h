// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "GA_MeleeAttackBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_MeleeAttackBase : public UGA_MontageAbility
{
	GENERATED_BODY()

public:
	UGA_MeleeAttackBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

	void TraceTick();
	FTimerHandle TimerHandle_TraceTick;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters")
	float TraceTickValue = 0.01f;

	bool TraceForHostileUnits(TArray<FHitResult>& OutHitResults);

	void AttackLogic(TArray<FHitResult>& OutHitResults);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	class AGAS_CharacterBase* CharacterBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameters")
	TSubclassOf<UGameplayEffect> GEPhysicalDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameters")
	FGameplayTagContainer TagsToAddToPhysicalDamageEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameters")
    FScalableFloat Damage;

};

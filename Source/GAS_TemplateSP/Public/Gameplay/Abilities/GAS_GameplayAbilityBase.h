// Qhax's GAS Template for 2D SinglePlayer

#pragma once

#include "Abilities/GameplayAbility.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Attributes/AS_Base.h"
#include "GAS_GameplayAbilityBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityLevelChanged, UGameplayAbility*, Ability, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCostChanged, UGameplayAbility*, Ability, float, NewCost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCooldownChanged, UGameplayAbility*, Ability, float, NewCooldown);


UCLASS()
class GAS_TEMPLATESP_API UGAS_GameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void StartupEffects();

	virtual void ApplyGameplayEffectToSelf(TArray<TSubclassOf<UGameplayEffect>> AbilityActivationEffects);

	UFUNCTION(BlueprintPure, Category = "GameplayAbilityBase")
	float GetCost(int32 AbilityLevel) const;

	UFUNCTION(BlueprintPure, Category = "GameplayAbilityBase")
	float GetCoolDown(int32 AbilityLevel) const;

	UPROPERTY(BlueprintAssignable, Category = "GameplayAbilityBase|Delegates")
	FOnAbilityLevelChanged OnAbilityLevelChanged;

	UPROPERTY(BlueprintAssignable, Category = "GameplayAbilityBase|Delegates")
	FOnAbilityCostChanged OnAbilityCostChanged;

	UPROPERTY(BlueprintAssignable, Category = "GameplayAbilityBase|Delegates")
	FOnAbilityCooldownChanged OnAbilityCooldownChanged;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbilityBase|TraceData")
	class UGAS_AbilityTraceData* TraceData;

	// Icon of ability that will seen on Gameplay UI
	UPROPERTY(EditAnywhere, Category = "GameplayAbilityBase|UI")
	class UTexture2D* AbilityIcon;

	UPROPERTY(EditAnywhere, Category = "GameplayAbilityBase")
	TArray<TSubclassOf<UGameplayEffect>> AbilityActivationEffects;

protected:
	/**
	 * For "Instanced Per Actor" abilities:
	 * Normally, broadcasting delegates over the CDO is not required for proper functionality. 
	 * However, since the "FindAbilitySpecFromClass" function is used immediately after granting the ability, 
	 * it returns the CDO instead of the instance object. As a result, we use the CDO here to ensure that
	 * delegates remain connected and can be broadcast over the same object.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void IncreaseLevel(UAbilitySystemComponent* AbilitySystemComp);

};

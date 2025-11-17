// Qhax's GAS Template for 2D SinglePlayer

#pragma once

#include "Abilities/GameplayAbility.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Attributes/AS_Base.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "GAS_GameplayAbilityBase.generated.h"

class UGAS_GameplayAbilityBase;

USTRUCT()
struct FCustomAbilityEndedData
{
	GENERATED_USTRUCT_BODY()

	FCustomAbilityEndedData()
		: AbilityThatEnded(nullptr)
		, bWasCancelled(false)
	{
	}

	FCustomAbilityEndedData(UGAS_GameplayAbilityBase* InAbility, bool bInWasCancelled)
		: AbilityThatEnded(InAbility)
		, bWasCancelled(bInWasCancelled)
	{
	}

	/** Ability that ended, normally instance but could be CDO */
	UGAS_GameplayAbilityBase* AbilityThatEnded;

	/** True if this was cancelled deliberately, false if it ended normally */
	bool bWasCancelled;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityLevelChanged, UGameplayAbility*, Ability, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCostChanged, UGameplayAbility*, Ability, float, NewCost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCooldownChanged, UGameplayAbility*, Ability, float, NewCooldown);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityEnded, const FCustomAbilityEndedData&);

UCLASS(BlueprintType)
class GAS_TEMPLATESP_API UGAS_GameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGAS_GameplayAbilityBase();

	FORCEINLINE UGAS_AbilitySystemComponent* GetASC() const
	{
		return CastChecked<UGAS_AbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get());
	}

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
	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbilityBase|UI")
	class UTexture2D* AbilityIcon;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbilityBase")
	TArray<TSubclassOf<UGameplayEffect>> AbilityActivationEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAbilityBase")
	float MinRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAbilityBase")
	float MaxRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAbilityBase")
	bool bApplyCommit = true;

	FOnAbilityEnded OnAbilityEnded;

	// Returns true if any of the ability's cooldown tags are currently active on the given ASC.
    // NOTE: This function is intended to be used with the CDO of the ability, so a valid ASC must be provided.
	bool IsOnCooldown(UAbilitySystemComponent* ASC);

	void EndAbilityManually();

protected:
	UPROPERTY()
	AGAS_CharacterBase* CharacterBase;

	/**
	 * For "Instanced Per Actor" abilities:
	 * Normally, broadcasting delegates over the CDO is not required for proper functionality. 
	 * However, since the "FindAbilitySpecFromClass" function is used immediately after granting the ability, 
	 * it returns the CDO instead of the instance object. As a result, we use the CDO here to ensure that
	 * delegates remain connected and can be broadcast over the same object.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void IncreaseLevel(UAbilitySystemComponent* AbilitySystemComp);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

};

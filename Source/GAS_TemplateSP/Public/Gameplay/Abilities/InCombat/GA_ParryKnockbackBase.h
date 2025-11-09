// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "GA_ParryKnockbackBase.generated.h"

// Struct to map an attack type to a knockback strength
USTRUCT(BlueprintType)
struct FAttackTypeKnockbackPair
{
    GENERATED_BODY()

    // The type tag of the incoming attack (e.g., Light, Heavy, Special)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Gameplay.Ability.Combat.Attack.Type"))
    FGameplayTag InComingAttackTypeTag;

    // Knockback force magnitude applied when this type is parried
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float KnockbackForce = 0.f;
};

// These knockback values are valid when parrying an attack.
// The force is resolved based on the incoming attack’s type tag.
UCLASS(BlueprintType)
class GAS_TEMPLATESP_API UAttackTypeToKnockbackAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    // List of attack type → knockback force mappings
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ParryKnockback")
    TArray<FAttackTypeKnockbackPair> AttackTypeKnockbackMappings;

    // Helper to lookup knockback force from attack type tag
    float FindKnockbackForce(FGameplayTag InComingAttackTypeTag) const
    {
        for (const FAttackTypeKnockbackPair& Pair : AttackTypeKnockbackMappings)
        {
            if (Pair.InComingAttackTypeTag == InComingAttackTypeTag)
            {
                return Pair.KnockbackForce;
            }
        }
        return 0.f;
    }
};
 
class UGA_MeleeAttackBase;

UCLASS()
class GAS_TEMPLATESP_API UGA_ParryKnockbackBase : public UGA_MontageAbility
{
	GENERATED_BODY()

public:
	UGA_ParryKnockbackBase();

    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    /**
    * Retrieves the attack type tag from the incoming melee attack.
    * Each melee attack ability contains its own AttackType tag (Light, Heavy, etc.).
    */
    FGameplayTag GetAttackTypeTagFromMeleeAttack(const UGA_MeleeAttackBase* MeleeAttack);

	UPROPERTY(EditDefaultsOnly, Category = "ParryKnockback")
	TSubclassOf<UGameplayEffect> GE_ParryKnockbackClass;

    UPROPERTY(EditDefaultsOnly, Category = "ParryKnockback")
    TSubclassOf<UGameplayEffect> GE_ParryKnockbackCostClass;

    UPROPERTY(EditDefaultsOnly, Category = "ParryKnockback")
    UAttackTypeToKnockbackAsset* KnockbackDataAsset;
};

// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "GA_TakeDamageBase.generated.h"

// Struct to map an incoming attack direction to the reaction montage
USTRUCT(BlueprintType)
struct FAttackDirectionHitMontagePair
{
    GENERATED_BODY()

    // The direction tag of the incoming attack
    // Each ability will carry its own AttackDirection tag
    // This is used to find the appropriate hit montage for the character
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Gameplay.Ability.Combat.Attack.Direction"))
    FGameplayTag InComingAttackDirectionTag;

    // Reaction montage we want to play
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UAnimMontage> HitMontage;
};

// These hit montages are valid when the character is facing the target (i.e., target is locked)
// If the character’s back is turned, direction tags are adjusted automatically 
UCLASS(BlueprintType)
class GAS_TEMPLATESP_API UAttackToHitMontageAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    // List of attack direction -> hit montage mappings
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
    TArray<FAttackDirectionHitMontagePair> AttackDirectionHitMappings;

    // Helper to lookup hit montage from attack direction tag
    UAnimMontage* FindHitMontage(FGameplayTag InComingAttackDirectionTag) const
    {
        for (const FAttackDirectionHitMontagePair& Pair : AttackDirectionHitMappings)
        {
            if (Pair.InComingAttackDirectionTag == InComingAttackDirectionTag)
            {
                return Pair.HitMontage.LoadSynchronous();
            }
        }
        return nullptr;
    }
};

class UGA_MeleeAttackBase;

UCLASS()
class GAS_TEMPLATESP_API UGA_TakeDamageBase : public UGA_MontageAbility
{
	GENERATED_BODY()

public:
	UGA_TakeDamageBase();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    /**
    * Retrieves the attack direction tag from the incoming melee attack.
    * Each melee attack ability contains its own AttackDirection tag (LeftToRight, RightToLeft, etc.).
    */
    FGameplayTag GetDirectionTagFromMeleeAttack(const UGA_MeleeAttackBase* MeleeAttack);

    /**
    * Adjusts the incoming attack direction tag based on the character's orientation.
    * If the character is facing away from the attack (back turned), LeftToRight becomes RightToLeft and vice versa.
    * This ensures the selected hit montage still visually corresponds to the attack direction.
    */
    FGameplayTag GetAdjustedAttackDirectionTag(FGameplayTag InComingAttackDirection, const AActor* Instigator);

	UAnimMontage* GetHitMontage(FGameplayTag InComingAttackDirectionTag);

    UPROPERTY(EditDefaultsOnly, Category = "TakeDamageBase")
    UAttackToHitMontageAsset* ReactionDataAsset;
};

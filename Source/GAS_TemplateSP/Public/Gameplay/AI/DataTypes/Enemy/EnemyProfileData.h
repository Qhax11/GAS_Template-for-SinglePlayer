// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "EnemyProfileData.generated.h"

/**
 * Defines static, profile-level combat permissions for an enemy.
 *
 * This data represents what the enemy is fundamentally allowed to do
 * (e.g. attack, parry, dodge, block) and is applied once on spawn
 * to configure AI behavior via gameplay tags.
 *
 * These rules are NOT dynamic states or decisions:
 * - They do not change during combat (except for explicit phase switches).
 * - They are not used for scoring or selection logic.
 *
 * The profile is converted into gameplay tags on BeginPlay,
 * and all AI decision / reaction data types query those tags
 * to determine availability.
 */
USTRUCT(BlueprintType)
struct FEnemyCombatRules
{
    GENERATED_BODY()

    /** Whether this enemy is allowed to perform any attack actions. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bCanAttack = true;

    /** Whether this enemy is allowed to react with parry mechanics. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bCanParry = true;

    /** Whether this enemy is allowed to use dodge-based reactions or movement. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bCanDodge = true;

    /** Whether this enemy is allowed to block incoming attacks. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bCanBlock = true;

    /** Whether this enemy is allowed to use movement chains or advanced movement logic. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bCanUseMovementChains = true;
};

/**
 * High-level enemy configuration asset defining combat capabilities.
 *
 * EnemyProfileData describes WHAT an enemy can do, not HOW it decides to do it.
 * It is owned by the enemy actor and applied once during initialization.
 *
 * This asset intentionally contains only binary permissions (rules).
 * More complex behavior (scoring, decision-making, reactions)
 * is handled by separate data assets and AI systems.
 */
UCLASS(BlueprintType)
class GAS_TEMPLATESP_API UEnemyProfileData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FEnemyCombatRules CombatRules;
};
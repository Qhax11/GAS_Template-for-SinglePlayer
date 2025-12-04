// Qhax's GAS Template for SinglePlayer

#pragma once

#include "GameplayTagContainer.h"
#include "CombatTypes.generated.h"

/**
 * FComingAttackPayload
 *
 * Generic gameplay payload describing an incoming attack event.
 * This struct is used to communicate attack-related information to systems such as
 * UI reactions, parry logic, hit prediction, or animation notifies.
 *
 * Key points:
 * - Contains the ability instance representing the incoming attack.
 * - Includes the predicted hit time used for defensive timing.
 * - Provides gameplay tags describing the attack’s properties.
 *
 * This type belongs under general Combat data definitions because it represents
 * runtime combat state, independent of AI decision-making or behavior logic.
 */

USTRUCT(BlueprintType)
struct FComingAttackPayload
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UGAS_GameplayAbilityBase* ComingAttack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ComingAttackHitTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTagContainer ComingAttackTags;

	FComingAttackPayload()
		: ComingAttack(nullptr)
	{}

	FComingAttackPayload(UGAS_GameplayAbilityBase* InComingAttack, float InComingAttackHitTime, FGameplayTagContainer InComingAttackTags)
		: ComingAttack(InComingAttack), ComingAttackHitTime(InComingAttackHitTime), ComingAttackTags(InComingAttackTags)
	{}
};
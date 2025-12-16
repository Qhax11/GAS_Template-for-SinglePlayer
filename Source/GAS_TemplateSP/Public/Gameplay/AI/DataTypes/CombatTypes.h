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

class UGAS_GameplayAbilityBase;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FComingAttackPayload
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UGAS_GameplayAbilityBase* ComingAttack;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ComingAttackHitTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTagContainer ComingAttackTags;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag AttackTypeTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AttackDirectionTag;

	UPROPERTY(BlueprintReadOnly)
	AActor* Attacker;

	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* AttackerASC;

	UPROPERTY(BlueprintReadOnly)
	AActor* Defender;

	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* DefenderASC;


	FComingAttackPayload()
		: ComingAttack(nullptr)
	{}

	FComingAttackPayload(UGAS_GameplayAbilityBase* InComingAttack, 
		float InComingAttackHitTime,
		FGameplayTagContainer InComingAttackTags,
		FGameplayTag InAttackTypeTag,
		FGameplayTag InAttackDirectionTag,
		AActor* InAttacker,
		UAbilitySystemComponent* InAttackerASC,
		AActor* InDefender,
		UAbilitySystemComponent* InDefenderASC)
		:
		ComingAttack(InComingAttack), 
		ComingAttackHitTime(InComingAttackHitTime), 
		ComingAttackTags(InComingAttackTags),
		AttackTypeTag(InAttackTypeTag) ,
		AttackDirectionTag(InAttackDirectionTag),
		Attacker(InAttacker),
		AttackerASC(InAttackerASC),
		Defender(InDefender),
		DefenderASC(InDefenderASC)
	{}
};
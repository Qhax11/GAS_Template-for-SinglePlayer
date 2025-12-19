// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/StaticDelegates/S_DamageDelegates.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionData.h"

class UAttackDataBase;
class UMovementDataBase;
class UMovementChainData;
class UAC_EnemyMovementManager;
class UMovementChainAsset;
class UGAS_GameplayAbilityBase;

struct FStatePayloadBase
{
    virtual ~FStatePayloadBase() {} // Virtual destructor is essential!
};

struct FMovementStatePayload : public FStatePayloadBase
{
	UMovementChainData* SelectedMovementChainData = nullptr;
	UAttackDataBase* SelectedAttackData = nullptr; 

	FMovementStatePayload(
		UMovementChainData* InSelectedMovementChainData,
		UAttackDataBase* InSelectedAttackData)
		: 
		SelectedMovementChainData(InSelectedMovementChainData),
		SelectedAttackData(InSelectedAttackData)
	{
	}
};

struct FAttackStatePayload : public FStatePayloadBase
{
	UAttackDataBase* AttackData;

	FAttackStatePayload(UAttackDataBase* InAttackData)
		: AttackData(InAttackData) {
	}
};

struct FIncomingAttackStatePayload : public FStatePayloadBase
{
	FComingAttackPayload AttackPayload;
	UComingAttackReactionData* ReactionData = nullptr;

	FIncomingAttackStatePayload(FComingAttackPayload InPayload, UComingAttackReactionData* InReaction)
		: AttackPayload(InPayload), ReactionData(InReaction) {
	}
};

struct FTakeHitStatePayload : public FStatePayloadBase
{
	FDamageData DamageData;

	FTakeHitStatePayload(FDamageData InDamageData)
		: DamageData(InDamageData) {
	}
};

struct FStateTransitionRequest
{
	FGameplayTag SourceStateTag;
	FGameplayTag TargetStateTag;
	TSharedPtr<FStatePayloadBase> Payload;

	FStateTransitionRequest(FGameplayTag InSourceStateTag, 
		FGameplayTag InTargetTag, 
		TSharedPtr<FStatePayloadBase> InPayload)
		: 
		SourceStateTag(InSourceStateTag),
		TargetStateTag(InTargetTag), 
		Payload(InPayload) 
	{
	}

	FStateTransitionRequest() = default;
};
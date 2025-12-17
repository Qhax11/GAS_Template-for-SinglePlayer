// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"
#include "Gameplay/StaticDelegates/S_DamageDelegates.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionData.h"

class UAttackDataBase;
class UAC_EnemyMovementManager;
class UMovementChainAsset;
class UGAS_GameplayAbilityBase;

struct FStatePayloadBase
{
    virtual ~FStatePayloadBase() {} // Virtual destructor is essential!
};

struct FMovementStatePayload : public FStatePayloadBase
{
	UMovementChainAsset* MovementChainAsset = nullptr;

	UAttackDataBase* SelectedAttackData = nullptr; 

	FMovementStatePayload(UMovementChainAsset* InMovementChainAsset, UAttackDataBase* InSelectedAttackData)
		: MovementChainAsset(InMovementChainAsset), SelectedAttackData(InSelectedAttackData) {
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
	FGameplayTag TargetStateTag;
	TSharedPtr<FStatePayloadBase> Payload;

	FStateTransitionRequest(FGameplayTag InTargetTag, TSharedPtr<FStatePayloadBase> InPayload)
		: TargetStateTag(InTargetTag), Payload(InPayload) {
	}

	FStateTransitionRequest() = default;
};
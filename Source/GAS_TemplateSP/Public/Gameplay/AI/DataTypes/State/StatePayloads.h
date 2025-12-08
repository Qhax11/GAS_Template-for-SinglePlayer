// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/AI/DataTypes/Behavior/AttackData.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionData.h"


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

	TSubclassOf<UGAS_GameplayAbilityBase> TargetAttackClass = nullptr; // Range check için

	FMovementStatePayload(UMovementChainAsset* InMovementChainAsset, TSubclassOf<UGAS_GameplayAbilityBase> InTargetAttackClass)
		: MovementChainAsset(InMovementChainAsset), TargetAttackClass(InTargetAttackClass) {
	}
};

struct FAttackStatePayload : public FStatePayloadBase
{
	FAttackData AttackData;

	FAttackStatePayload(FAttackData InAttackData)
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
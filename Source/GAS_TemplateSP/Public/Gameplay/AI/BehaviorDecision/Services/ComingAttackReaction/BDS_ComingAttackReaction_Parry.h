// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReactionBase.h"
#include "BDS_ComingAttackReaction_Parry.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UBDS_ComingAttackReaction_Parry : public UBDS_ComingAttackReactionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGAS_GameplayAbilityBase> ParryAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCounterImmediatelyAfterParry = false;
};

// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Cues/GCN_ActorBase.h"
#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/Components/AC_GameplayData.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "GCN_CombatBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AGCN_CombatBase : public AGCN_ActorBase
{
	GENERATED_BODY()
	
public:
	virtual bool OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters) override;

protected:
	const UGA_MeleeAttackBase* SourceMeleeAttack;
	FGameplayTag SourceMeleeAttackType;
	UAC_GameplayData* SourceDataComponent;
	UAbilitySystemComponent* SourceASC;
	UAbilitySystemComponent* TargetASC;
};

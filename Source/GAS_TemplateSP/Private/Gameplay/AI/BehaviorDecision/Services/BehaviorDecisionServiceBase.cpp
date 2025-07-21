// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/BehaviorDecisionServiceBase.h"

void UBehaviorDecisionServiceBase::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
	Enemy = BehaviorServiceInitParams.Enemy;
	EnemyController = BehaviorServiceInitParams.EnemyController;
	EnemyASC = BehaviorServiceInitParams.EnemyASC;
	Hero = BehaviorServiceInitParams.Hero;
	HeroMovementListenerComp = BehaviorServiceInitParams.HeroMovementListenerComp;
	BehaviorState = BehaviorServiceInitParams.BehaviorState;
	// The InAsset parameter will be cast to the appropriate asset type in subclasses.
}

void UBehaviorDecisionServiceBase::InitializeAfterSelection()
{
    // The logic will be implement in subclasses.
}

FGameplayTag UBehaviorDecisionServiceBase::GetRandomDirectionTag()
{
    static const TArray<FGameplayTag> PossibleDirections =
    {
        //GAS_Tags::TAG_AI_Direction_Resolved_Forward,
        GAS_Tags::TAG_AI_Direction_Resolved_Backward,
        GAS_Tags::TAG_AI_Direction_Resolved_Left,
        GAS_Tags::TAG_AI_Direction_Resolved_Right
    };

    int32 RandomIndex = FMath::RandRange(0, PossibleDirections.Num() - 1);
    return PossibleDirections[RandomIndex];
}

void UBehaviorDecisionServiceBase::ApplyDirectionPoliciesToMovementAbility(FMovementAbilityData& MovementAbilityData)
{
    if (!MovementAbilityData.DirectionPolicyTag.IsValid())
    {
        return;
    }

    FGameplayTag HeroLastDirectionGameplayTag = HeroMovementListenerComp->GetHeroLastMovementDirectionTagByLastInput();

    if (MovementAbilityData.DirectionPolicyTag == GAS_Tags::TAG_AI_Direction_Policy_PlayerLastDirection)
    {
        if (HeroLastDirectionGameplayTag.IsValid())
        {
            MovementAbilityData.ResolvedDirectionTag = HeroLastDirectionGameplayTag;
        }
    }
    else if (MovementAbilityData.DirectionPolicyTag == GAS_Tags::TAG_AI_Direction_Policy_Random)
    {
        MovementAbilityData.ResolvedDirectionTag = GetRandomDirectionTag();
    }
}


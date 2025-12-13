// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/Data/ComingAttackReactionDataDodge.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"


UComingAttackReactionDataDodge::UComingAttackReactionDataDodge()
{
    ComingAttackReactionName = FName("Dodge");
    ReactionType = EComingAttackReaction::Dodge;
    MinimumTimeBeforeHitToReact = 0.15f;    
    PreferredTriggerTimeBeforeHit = 0.25f;    
    BaseChance = 0.6f;

    // Dash distance
    DodgeMovementAbilityData.AbilityEventMagnitude = 300.0f;
    DodgeMovementAbilityData.ResolvedDirectionTag = GAS_Tags::TAG_AI_Direction_Resolved_Backward;
}

bool UComingAttackReactionDataDodge::IsEnable(UComingAttackReactionData* ComingReactionData, FComingAttackPayload ComingAttackPayload) const
{
    bool bIsUnDodgeableAttack = ComingAttackPayload.ComingAttackTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Type_Undodgeable);
    return Super::IsEnable(ComingReactionData, ComingAttackPayload) && !bIsUnDodgeableAttack;
}

FGameplayTag UComingAttackReactionDataDodge::GetRandomDirectionTag()
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

void UComingAttackReactionDataDodge::ApplyDirectionPoliciesToMovementAbility(UAC_HeroMovementListener* HeroMovementListener)
{
    if (!DodgeMovementAbilityData.DirectionPolicyTag.IsValid())
    {
        return;
    }

    FGameplayTag HeroLastDirectionGameplayTag = HeroMovementListener->GetHeroLastMovementDirectionTagByLastInput();

    if (DodgeMovementAbilityData.DirectionPolicyTag == GAS_Tags::TAG_AI_Direction_Policy_PlayerLastDirection)
    {
        if (HeroLastDirectionGameplayTag.IsValid())
        {
            DodgeMovementAbilityData.ResolvedDirectionTag = HeroLastDirectionGameplayTag;
        }
    }
    else if (DodgeMovementAbilityData.DirectionPolicyTag == GAS_Tags::TAG_AI_Direction_Policy_Random)
    {
        DodgeMovementAbilityData.ResolvedDirectionTag = GetRandomDirectionTag();
    }
}

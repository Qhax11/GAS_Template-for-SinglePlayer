// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/BDS_ComingAttackReaction.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"

void UBDS_ComingAttackReaction::Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams)
{
    Super::Initialize(BehaviorServiceInitParams);

    UComingAttackReactionAsset* CastedAsset = Cast<UComingAttackReactionAsset>(BehaviorServiceInitParams.Asset);
    if (CastedAsset)
    {
        ComingAttackReaction = CastedAsset;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid asset type passed to UComingAttackReactionService!"));
    }
}

EComingAttackReaction UBDS_ComingAttackReaction::GetComingAttackDecision(FComingAttackPayload ComingAttackPayload)
{
    // If ability is null, default to taking the hit
    if (!ComingAttackPayload.ComingAttack)
    {
        return EComingAttackReaction::TakeDamage;
    }

    const FGameplayTagContainer& ComingAttackTags = ComingAttackPayload.ComingAttackTags;

    // Shadow attacks cannot be parried
    if (ComingAttackTags.HasTag(GAS_Tags::TAG_Gameplay_Ability_Attack_MeleeCombo_ShadowLinked))
    {
        return EComingAttackReaction::Dodge;
    }

    // Default fallback
    return EComingAttackReaction::TakeDamage;
}

void UBDS_ComingAttackReaction::SetComingAttackReactionAsset(UComingAttackReactionAsset* ComingAttackReactionAsset)
{
    if (!ComingAttackReactionAsset) 
    {
        UE_LOG(LogTemp, Warning, TEXT("ComingAttackReactionAsset is null in: %s"), *GetName());
        return;
    }

    ComingAttackReaction = ComingAttackReactionAsset;
}

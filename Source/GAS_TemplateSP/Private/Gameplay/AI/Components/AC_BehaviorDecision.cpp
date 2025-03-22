// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/AI/StateTree/ST_Base.h"

UAC_BehaviorDecision::UAC_BehaviorDecision()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_BehaviorDecision::BeginPlay()
{
	Super::BeginPlay();

    OwnerController = Cast<AAIControllerBase>(GetOwner());
    if (!OwnerController) 
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerController is null in: %s !"), *GetName());
    }
}

FAttackAbilityData UAC_BehaviorDecision::GetBestAttack(float DistanceToTarget)
{
    if (!AttackAbilityData)
    {
        return FAttackAbilityData(); // Empty
    }

    float BestScore = -FLT_MAX;
    FAttackAbilityData BestAttack;

    for (const FAttackAbilityData& Attack : AttackAbilityData->AttackAbilities)
    {
        if (!Attack.AbilityClass) 
        {
            continue;
        }

        // DistanceScore
        float IdealDistance = Attack.MaxRange;
        float DistanceFactor = 1.f - (DistanceToTarget - IdealDistance) / IdealDistance;

        float TotalScore = DistanceFactor;

        // Debug
        UE_LOG(LogTemp, Log, TEXT("[AI] Attack %s → Score: %.1f"), *Attack.AbilityClass->GetName(), TotalScore);

        if (TotalScore > BestScore)
        {
            BestScore = TotalScore;
            BestAttack = Attack;
        }
    }

    SelectedAttackAbilityData = BestAttack;
    return BestAttack;
}

void UAC_BehaviorDecision::SendSelectedAttackData()
{
    if (OwnerController) 
    {
        OwnerController->GetStateTreeComponent()->SendStateTreeEvent(
            GAS_Tags::TAG_AI_StateTreeEvent_ExecuteSelectedAttack, FConstStructView::Make(SelectedAttackAbilityData));

    }
}


// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/EQS/Tests/EQT_DotDirection.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"

UEQT_DotDirection::UEQT_DotDirection()
{
}

void UEQT_DotDirection::RunTest(FEnvQueryInstance& QueryInstance) const
{
    TArray<AActor*> ContextActors;
    QueryInstance.PrepareContext(TargetActorContext, ContextActors);

    if (ContextActors.Num() == 0 || !ContextActors[0])
    {
        return;
    }

    const AActor* ReferenceActor = ContextActors[0];

    float Multiplier = GetBoundFloatValue(QueryInstance, ScoringMultiplier);
    float MaxScore = GetBoundFloatValue(QueryInstance, MaxExpectedScore);
    /*
    AAIControllerBase* AIController = Cast<AAIControllerBase>(QueryInstance.Owner.Get());
    if (!AIController)
    {
        return;
    }

    APawn* QuererPawn = AIController->GetPawn();
    if (!QuererPawn)
    {
        return;
    }
    */
    EStrafeDirection StrafeDirection = GetDirectionFromParam(QueryInstance);

    FVector QuererPawnLocation = ReferenceActor->GetActorLocation();
    FVector QuererPawnForward = ReferenceActor->GetActorForwardVector();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        FVector QueryPoint = GetItemLocation(QueryInstance, It.GetIndex());

        FVector QuererLocationToPointDirection = QueryPoint - QuererPawnLocation;

        if (QuererLocationToPointDirection.IsNearlyZero())
        {
            continue;
        }

        FVector BossLocationToPointDirectionNormalized = QuererLocationToPointDirection.GetSafeNormal();

        float CrossZ = FVector::CrossProduct(QuererPawnForward, BossLocationToPointDirectionNormalized).Z;
        bool bIsRight = (CrossZ > 0.0f);  // Pozitifse sağ, negatifse sol

        // ✅ Enum'a göre yön filtresi (sadece doğru yönü skorla)
        if ((StrafeDirection == EStrafeDirection::Left && bIsRight) ||
            (StrafeDirection == EStrafeDirection::Right && !bIsRight))
        {
            It.SetScore(EEnvTestPurpose::Score, EEnvTestFilterType::Range, 0.0f, 0.0f, 1.0f);
            continue; // Yanlış yön → atla
        }

        // 2️⃣ **Açıya Göre Skorlama (Dot Product ile)**
        float AngleScore = FVector::DotProduct(QuererPawnForward, BossLocationToPointDirectionNormalized);

        if (AngleScore >= FrontCutoffDotThreshold)
        {
            It.ForceItemState(EEnvItemStatus::Failed);
            continue;
        }

        // Ön taraf kötü, sağ/sol uç noktalar iyi (Strafing için)
        float StrafingScore = 1.0f;
        StrafingScore = ApplyFrontPenalty(StrafingScore, AngleScore);

        StrafingScore *= Multiplier;
        It.SetScore(EEnvTestPurpose::Score, EEnvTestFilterType::Range, StrafingScore, 0.0f, MaxScore);
    }
}

EStrafeDirection UEQT_DotDirection::GetDirectionFromParam(const FEnvQueryInstance& QueryInstance) const
{
    float DirectionValue = 2.0f;
    if (const float* FoundValue = QueryInstance.NamedParams.Find(FName("StrafeDirectionParam")))
    {
        DirectionValue = *FoundValue;
    }

    EStrafeDirection StrafeDirection = EStrafeDirection::Both;
    if (DirectionValue == 0.0f)
    {
        StrafeDirection = EStrafeDirection::Left;
    }
    else if (DirectionValue == 1.0f)
    {
        StrafeDirection = EStrafeDirection::Right;
    }

    return StrafeDirection;
}

float UEQT_DotDirection::ApplyFrontPenalty(float StrafingScore, float AngleDot) const
{
    if (AngleDot > 0.0f)
    {
        const float PenaltyStrength01 = FMath::Clamp(FrontPenaltyStrength / 100.0f, 0.0f, 1.0f);
        const float Penalty = AngleDot * PenaltyStrength01;

        StrafingScore -= Penalty;
        StrafingScore = FMath::Clamp(StrafingScore, 0.0f, 1.0f);
    }

    return StrafingScore;
}

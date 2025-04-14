// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/EQS/Tests/EQT_DistanceToPoints.h"

void UEQT_DistanceToPoints::RunTest(FEnvQueryInstance& QueryInstance) const
{
    Super::RunTest(QueryInstance);

    if (!QuerierActor)
    {
        return;
    }

    const FVector QuerierLocation = QuerierActor->GetActorLocation();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const FVector PointLocation = GetItemLocation(QueryInstance, It.GetIndex());

        const float DistanceQuerierToPoint = FVector::Dist(QuerierLocation, PointLocation);

        // ❌ Filter: mesafe aralık dışında → elenir (ama sadece bu sorguda)
        if (DistanceQuerierToPoint < MinDistance || DistanceQuerierToPoint > MaxDistance)
        {
            //It.ForceItemState(EEnvItemStatus::Failed);
            SetScoreToItem(It, 0.0f);
        }

        // ✅ Score hesaplama
        float Normalized = FMath::Clamp((DistanceQuerierToPoint - MinDistance) / (MaxDistance - MinDistance), 0.0f, 1.0f);

        float FinalScore = 0.0f;

        switch (ScoringMode)
        {
        case EDistanceScoringMode::Linear:
            FinalScore = 1.0f - Normalized; // Yakın = yüksek skor
            break;
        case EDistanceScoringMode::InverseLinear:
            FinalScore = Normalized; // Uzak = yüksek skor
            break;
        }

        FinalScore *= ScoringMultiplier;
        SetScoreToItem(It, FinalScore);
    }
}

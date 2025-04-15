// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/EQS/Tests/EQT_DistanceToPoints.h"

void UEQT_DistanceToPoints::RunTest(FEnvQueryInstance& QueryInstance) const
{
    Super::RunTest(QueryInstance);

  

    TArray<FVector> ContextLocations;
    QueryInstance.PrepareContext(QuerierActorContext, ContextLocations);

    if (ContextLocations.Num() == 0 || !ContextLocations.IsValidIndex(0))
    {
        return;
    }

    const FVector QuerierLocation = ContextLocations[0];


    //const FVector QuerierLocation = QuerierActor->GetActorLocation();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const FVector PointLocation = GetItemLocation(QueryInstance, It.GetIndex());

        const float DistanceQuerierToPoint = FVector::Dist(QuerierLocation, PointLocation);

        // ❌ Filter: mesafe aralık dışında → elenir (ama sadece bu sorguda)
        if (DistanceQuerierToPoint < MinDistance || DistanceQuerierToPoint > MaxDistance)
        {
            It.ForceItemState(EEnvItemStatus::Failed);
            //SetScoreToItem(It, 0.0f);
            continue;
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

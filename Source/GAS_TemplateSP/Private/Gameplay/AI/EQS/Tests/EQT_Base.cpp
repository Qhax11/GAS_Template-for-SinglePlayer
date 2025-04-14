// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/EQS/Tests/EQT_Base.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"

UEQT_Base::UEQT_Base()
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
    SetWorkOnFloatValues(true);

    QuerierActorContext = UEnvQueryContext_Querier::StaticClass();
}

void UEQT_Base::RunTest(FEnvQueryInstance& QueryInstance) const
{
    TArray<AActor*> ContextActors;
    QueryInstance.PrepareContext(QuerierActorContext, ContextActors);

    if (ContextActors.Num() == 0 || !ContextActors.IsValidIndex(0))
    {
        return;
    }

    QuerierActor = ContextActors[0];
}

void UEQT_Base::SetScoreToItem(FEnvQueryInstance::ItemIterator& Item, float Score) const
{
    Item.SetScore(EEnvTestPurpose::Score, EEnvTestFilterType::Range, Score, 0.0f, MaxExpectedScore);
}



// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/EQS/Tests/EQT_Base.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"

UEQT_Base::UEQT_Base()
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
    SetWorkOnFloatValues(true);

    TargetActorContext = UEnvQueryContext_Querier::StaticClass();

    ScoringMultiplier.DefaultValue = 1.0f;
    MaxExpectedScore.DefaultValue = 3.0f;
}

void UEQT_Base::RunTest(FEnvQueryInstance& QueryInstance) const
{
}

float UEQT_Base::GetBoundFloatValue(const FEnvQueryInstance& QueryInstance, const FAIDataProviderFloatValue& Value) const
{
    const_cast<FAIDataProviderFloatValue&>(Value).BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);
    return Value.GetValue();
}

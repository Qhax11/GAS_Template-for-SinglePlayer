// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "DecisionOptionData.generated.h"


UCLASS(Blueprintable, DefaultToInstanced, EditInLineNew, Abstract)
class GAS_TEMPLATESP_API UDecisionOptionData : public UObject
{
	GENERATED_BODY()

public:
	// Hard gate
	virtual bool IsEnable(const void* Context, void* OutDebug = nullptr) const
	{
		return true;
	}

	// Probabilistic gate
	virtual bool PassesChance(const void* Context, void* OutDebug = nullptr) const
	{
		return true;
	}

	// Deterministic priority
	virtual float GetScore(const void* Context, void* OutDebug = nullptr) const
	{
		return 0.f;
	}
	
};

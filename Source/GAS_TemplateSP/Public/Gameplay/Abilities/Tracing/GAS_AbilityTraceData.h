// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Tracing/GAS_TraceBase.h"
#include "GAS_AbilityTraceData.generated.h"


UCLASS(Blueprintable)
class GAS_TEMPLATESP_API UGAS_AbilityTraceData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Targeting")
	TObjectPtr<UGAS_TraceBase> Trace;

};
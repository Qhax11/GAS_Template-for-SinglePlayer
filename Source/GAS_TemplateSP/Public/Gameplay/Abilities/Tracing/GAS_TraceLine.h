// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Tracing/GAS_TraceBase.h"
#include "GAS_TraceLine.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGAS_TraceLine : public UGAS_TraceBase
{
	GENERATED_BODY()
	
protected:

	virtual void TraceLogic(const UWorld* World, const FTraceRequest& TraceRequest, const FCollisionQueryParams& QueryParams, const FCollisionResponseParams& ResponseParams, TArray<FHitResult>& OutHitResults) override;

#if WITH_EDITOR
	virtual void DrawDebugShape(const UWorld* World, const FTraceRequest& TraceRequest) const override;
#endif // WITH_EDITOR
};

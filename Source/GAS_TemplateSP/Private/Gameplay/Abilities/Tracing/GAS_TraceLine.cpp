// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Tracing/GAS_TraceLine.h"

void UGAS_TraceLine::TraceLogic(const UWorld* World, const FTraceRequest& TraceRequest, const FCollisionQueryParams& QueryParams, const FCollisionResponseParams& ResponseParams, TArray<FHitResult>& OutHitResults)
{
	World->LineTraceMultiByChannel(
		OutHitResults,
		TraceRequest.StartLocation,
		TraceRequest.EndLocation,
		TraceChannel,
		QueryParams,
		ResponseParams
	);
}

#if WITH_EDITOR
void UGAS_TraceLine::DrawDebugShape(const UWorld* World, const FTraceRequest& TraceRequest) const
{
	DrawDebugLine(
		World,
		TraceRequest.StartLocation,
		TraceRequest.StartLocation + TraceRequest.Direction.Vector() * TraceDistance,
		DrawColor,
		false,
		DebugShapeDrawDuration,
		0,
		2.0f
	);
}
#endif // WITH_EDITOR

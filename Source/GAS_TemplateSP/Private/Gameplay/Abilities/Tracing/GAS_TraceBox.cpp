// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Tracing/GAS_TraceBox.h"

FCollisionShape UGAS_TraceBox::GetCollisionShape() const
{
    return FCollisionShape::MakeBox(FVector(HalfLength, HalfWidth, HalfHeight));
}

#if WITH_EDITOR
void UGAS_TraceBox::DrawDebugShape(const UWorld* World, const FTraceRequest& TraceRequest) const
{
	DrawDebugBox(
		World,
		TraceRequest.StartLocation + TraceRequest.Direction.Vector() * HalfLength,
		FVector(HalfLength, HalfWidth, HalfHeight),
		TraceRequest.Direction.Quaternion(),
		DrawColor,
		false,
		DebugShapeDrawDuration,
		0,
		2.0f
	);
}
#endif // WITH_EDITOR

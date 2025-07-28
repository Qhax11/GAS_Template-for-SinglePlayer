// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Tracing/GAS_TraceSphere.h"

FCollisionShape UGAS_TraceSphere::GetCollisionShape() const
{
    return FCollisionShape::MakeSphere(Radius);
}

#if WITH_EDITOR
void UGAS_TraceSphere::DrawDebugShape(const UWorld* World, const FTraceRequest& TraceRequest) const
{
	DrawDebugCapsule(
		World,
		TraceRequest.StartLocation + TraceRequest.Direction.Vector() * (TraceDistance / 2.0f),
		TraceDistance / 2.0f + Radius,
		Radius,
		FQuat::MakeFromEuler(FVector(.0f, 90.0f + TraceRequest.Direction.Euler().Y, TraceRequest.Direction.Euler().Z)),
		DrawColor,
		false,
		DebugShapeDrawDuration,
		0,
		2.0f
	);
}
#endif // WITH_EDITOR

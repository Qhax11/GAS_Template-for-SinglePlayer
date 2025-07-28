// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Tracing/GAS_TraceCapsule.h"
#include "KismetTraceUtils.h"


FCollisionShape UGAS_TraceCapsule::GetCollisionShape() const
{
    return FCollisionShape::MakeCapsule(Radius, HalfHeight);
}

#if WITH_EDITOR
void UGAS_TraceCapsule::DrawDebugShape(const UWorld* World, const FTraceRequest& TraceRequest) const
{
	DrawDebugCapsule(
		World,
		TraceRequest.StartLocation + TraceRequest.Direction.Vector().GetSafeNormal() * TraceDistance,
		HalfHeight,
		Radius,
		TraceRequest.Direction.Quaternion(),
		DrawColor,
		false,
		DebugShapeDrawDuration,
		0,
		2.0f
	);
}
#endif // WITH_EDITOR

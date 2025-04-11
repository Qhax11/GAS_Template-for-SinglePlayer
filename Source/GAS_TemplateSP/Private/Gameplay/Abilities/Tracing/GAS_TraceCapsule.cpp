// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Tracing/GAS_TraceCapsule.h"

FCollisionShape UGAS_TraceCapsule::GetCollisionShape() const
{
    return FCollisionShape::MakeCapsule(Radius, HalfHeight);
}

#if WITH_EDITOR
void UGAS_TraceCapsule::DrawDebugShape(const UWorld* World, const FVector& Location) const
{
	DrawDebugCapsule(
		World,
		Location + TraceDirection.Vector().GetSafeNormal() * TraceDistance,
		HalfHeight,
		Radius,
		TraceDirection.Quaternion(),
		DrawColor,
		false,
		DebugShapeDrawDuration,
		0,
		2.0f
	);
}
#endif // WITH_EDITOR

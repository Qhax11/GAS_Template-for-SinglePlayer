// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Tracing/GAS_TraceCapsule.h"
#include "KismetTraceUtils.h"


FCollisionShape UGAS_TraceCapsule::GetCollisionShape(const FTraceRequest& TraceRequest) const
{
    FVector TraceVector = TraceRequest.EndLocation - TraceRequest.StartLocation;
    float CapsuleHalfHeight = TraceVector.Size() * 0.5f;
    return FCollisionShape::MakeCapsule(Radius, CapsuleHalfHeight);
}

#if WITH_EDITOR
void UGAS_TraceCapsule::DrawDebugShape(const UWorld* World, const FTraceRequest& TraceRequest) const
{
    FVector TraceVector = TraceRequest.EndLocation - TraceRequest.StartLocation;
    FVector CapsuleCenter = (TraceRequest.StartLocation + TraceRequest.EndLocation) * 0.5f;
    float CapsuleHalfHeight = TraceVector.Size() * 0.5f;
    FQuat CapsuleQuat = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();

    DrawDebugCapsule(
        World,
        CapsuleCenter,
        CapsuleHalfHeight,
        Radius,
        CapsuleQuat,
        DrawColor,
        false,
        DebugShapeDrawDuration,
        0,
        2.0f
    );

	DrawDebugPoint(World, TraceRequest.StartLocation, 20.0f, FColor::Black, false, 0, 2.0f);
	DrawDebugPoint(World, TraceRequest.EndLocation, 20.0f, FColor::Black, false, 0, 2.0f);
}
#endif // WITH_EDITOR

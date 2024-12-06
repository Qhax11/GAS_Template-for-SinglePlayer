// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Tracing/GAS_TraceBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"


void UGAS_TraceBase::CreateTraceWithTeamFilter(const UWorld* World, AActor* Owner, ETeamAttitude::Type TeamAttidue, TArray<AActor*>& OutActors)
{
	FVector StartLocation;
	FRotator Direction;

	GetTraceStartLocationAndDirection(Owner, StartLocation, Direction);

	MakeTrace(Owner, World, StartLocation, Direction, OutActors);

	MakeTeamFilter(OutActors, *Owner, TeamAttidue);
}

void UGAS_TraceBase::CreateTraceWithTeamFilterAndDirection(const UWorld* World, AActor* Owner, ETeamAttitude::Type TeamAttidue, FRotator& Direction, TArray<AActor*>& OutActors)
{
	FVector StartLocation;

	GetTraceStartLocationAndDirection(Owner, StartLocation, Direction);

	MakeTrace(Owner, World, StartLocation, Direction, OutActors);

	MakeTeamFilter(OutActors, *Owner, TeamAttidue);
}

void UGAS_TraceBase::GetTraceStartLocationAndDirection(AActor* Owner, FVector& OutStartLocation, FRotator& OutDirection)
{
	// If TraceStartLocation is Camera, adjust the start location and direction based on Hero's camera
	if (TraceStartLocation == ETraceStartLocation::Camera)
	{
		if (AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(Owner))
		{
			OutStartLocation = HeroBase->GetFollowCamera()->GetComponentLocation();

			if (TraceDirectionType == ETraceDirectionType::ForwardVector)
			{
				OutDirection = HeroBase->GetFollowCamera()->GetForwardVector().Rotation();
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TraceOriginActor is set to Camera, but the current actor is not the hero!"));
		}
	}
	else if (TraceStartLocation == ETraceStartLocation::Avatar) 
	{
		OutStartLocation = Owner->GetActorLocation();

		if (TraceDirectionType == ETraceDirectionType::ForwardVector)
		{
			OutDirection = Owner->GetActorForwardVector().Rotation();
		}

	}
}

void UGAS_TraceBase::MakeTeamFilter(TArray<AActor*>& OutActors, const AActor& Owner, ETeamAttitude::Type TeamAttidue)
{
	for (AActor* CollectedActor : OutActors)
	{
		if (UAC_Team* TeamComp = CollectedActor->GetComponentByClass<UAC_Team>())
		{
			if (TeamComp->GetTeamAttitudeTowards(Owner) != TeamAttidue)
			{
				OutActors.Remove(CollectedActor);
			}
		}
		else
		{
			OutActors.Remove(CollectedActor);
		}
	}
}

void UGAS_TraceBase::MakeTrace(const UObject* Owner, const UWorld* World, const FVector& Location, const FRotator& Direction, TArray<AActor*>& OutActors)
{
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner can't be null! (This message comes from UGAS_TraceBase)"));
		return;
	}

	Initialize(Owner, Direction);

	FCollisionQueryParams QueryParams;
	if (bIgnoreSelf)
	{
		QueryParams.AddIgnoredActor(OwnerActor);
	}

	FCollisionResponseContainer ResponseContainer;
	ResponseContainer.SetAllChannels(CollisionResponse);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse = ResponseContainer;

	TArray<FHitResult> HitResults;
	TraceLogic(World, Location, Direction, QueryParams, ResponseParams, HitResults);

#if WITH_EDITOR
	if(bDrawEnable)
		DrawDebugShape(World, Location);
#endif // WITH_EDITOR

	if (HitResults.IsEmpty())
	{
		return;
	}

	for (int32 i = 0; i < (bSingleTarget ? 1 : HitResults.Num()); ++i)
	{
		OutActors.AddUnique(HitResults[i].GetActor());
	}
}

void UGAS_TraceBase::Initialize(const UObject* Owner, FRotator Direction)
{
	OwnerActor = Cast<AActor>(Owner);

	if (!bOverrideTraceDirection) 
	{
		TraceDirection = Direction;
	}

}

void UGAS_TraceBase::TraceLogic(const UWorld* World, const FVector& Location, const FRotator& Direction, const FCollisionQueryParams& QueryParams, const FCollisionResponseParams& ResponseParams, TArray<FHitResult>& OutHitResults)
{
	World->SweepMultiByChannel(
		OutHitResults,
		Location,
		Location + TraceDirection.Vector() * TraceDistance,
		FQuat::Identity,
		TraceChannel,
		GetCollisionShape(),
		QueryParams
	);
}

FCollisionShape UGAS_TraceBase::GetCollisionShape() const
{
    return FCollisionShape();
}

#if WITH_EDITOR
void UGAS_TraceBase::DrawDebugShape(const UWorld* World, const FVector& Location) const
{
	// Logic will be implmeneted in subclasses.
}
#endif // WITH_EDITOR

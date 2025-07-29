// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Tracing/GAS_TraceBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Kismet/KismetSystemLibrary.h"


void UGAS_TraceBase::CreateTraceWithTeamFilter(const UWorld* World, AActor* Owner, ETeamAttitude::Type TeamAttidue, TArray<AActor*>& OutActors, const FTraceRequest& TraceRequests)
{
	FVector StartLocation;
	FVector EndLocation;
	FRotator Direction;

	// StartLocation and Direction
	if (TraceStartLocation == ETraceStartLocation::Camera)
	{
		if (AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(Owner))
		{
			const FVector CameraLoc = HeroBase->GetFollowCamera()->GetComponentLocation();
			const FVector CameraForward = HeroBase->GetFollowCamera()->GetForwardVector();

			StartLocation = CameraLoc + CameraForward * StartLocationForwardOffset;

			if (TraceDirectionType == ETraceDirectionType::ForwardDirection)
			{
				Direction = CameraForward.Rotation();
			}
			else // Use original direction from TraceRequests
			{
				Direction = TraceRequests.Direction;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TraceStartLocation is set to Camera, but the actor is not HeroBase!"));
			StartLocation = TraceRequests.StartLocation;
			Direction = TraceRequests.Direction;
		}
	}
	else if (TraceStartLocation == ETraceStartLocation::Avatar)
	{
		StartLocation = Owner->GetActorLocation();

		if (TraceDirectionType == ETraceDirectionType::ForwardDirection)
		{
			Direction = Owner->GetActorForwardVector().Rotation();
		}
		else
		{
			Direction = TraceRequests.Direction;
		}
	}
	else // Default fallback (use given values)
	{
		StartLocation = TraceRequests.StartLocation;
		Direction = TraceRequests.Direction;
	}

	// EndLocation
	if (TraceEndLocation == ETraceEndLocation::ForwardVector)
	{
		EndLocation = StartLocation + Direction.Vector() * TraceDistance;
	}
	else // Use fixed EndLocation (e.g. socket or custom location)
	{
		EndLocation = TraceRequests.EndLocation;
	}

	// Final TraceRequest
	FTraceRequest TraceRequest;
	TraceRequest.StartLocation = StartLocation;
	TraceRequest.EndLocation = EndLocation;
	TraceRequest.Direction = Direction;

	// Perform trace
	MakeTrace(Owner, World, TraceRequest, OutActors);

	// Filter by team
	MakeTeamFilter(OutActors, *Owner, TeamAttidue);
}

void UGAS_TraceBase::CreateTraceWithTeamFilter(const UWorld* World, AActor* Owner, ETeamAttitude::Type TeamAttidue, TArray<FHitResult>& HitResults, const FTraceRequest& TraceRequests)
{
	FVector StartLocation;
	FVector EndLocation;
	FRotator Direction;

	// StartLocation and Direction
	if (TraceStartLocation == ETraceStartLocation::Camera)
	{
		if (AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(Owner))
		{
			const FVector CameraLoc = HeroBase->GetFollowCamera()->GetComponentLocation();
			const FVector CameraForward = HeroBase->GetFollowCamera()->GetForwardVector();

			StartLocation = CameraLoc + CameraForward * StartLocationForwardOffset;

			if (TraceDirectionType == ETraceDirectionType::ForwardDirection)
			{
				Direction = CameraForward.Rotation();
			}
			else // Use original direction from TraceRequests
			{
				Direction = TraceRequests.Direction;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TraceStartLocation is set to Camera, but the actor is not HeroBase!"));
			StartLocation = TraceRequests.StartLocation;
			Direction = TraceRequests.Direction;
		}
	}
	else if (TraceStartLocation == ETraceStartLocation::Avatar)
	{
		StartLocation = Owner->GetActorLocation();

		if (TraceDirectionType == ETraceDirectionType::ForwardDirection)
		{
			Direction = Owner->GetActorForwardVector().Rotation();
		}
		else
		{
			Direction = TraceRequests.Direction;
		}
	}
	else // Default fallback (use given values)
	{
		StartLocation = TraceRequests.StartLocation;
		Direction = TraceRequests.Direction;
	}

	// EndLocation
	if (TraceEndLocation == ETraceEndLocation::ForwardVector)
	{
		EndLocation = StartLocation + Direction.Vector() * TraceDistance;
	}
	else // Use fixed EndLocation (e.g. socket or custom location)
	{
		EndLocation = TraceRequests.EndLocation;
	}

	// Final TraceRequest
	FTraceRequest TraceRequest;
	TraceRequest.StartLocation = StartLocation;
	TraceRequest.EndLocation = EndLocation;
	TraceRequest.Direction = Direction;

	// Perform trace
	MakeTrace(Owner, World, TraceRequest, HitResults);

	// Filter by team
	MakeTeamFilter(HitResults, *Owner, TeamAttidue);
}

void UGAS_TraceBase::GetTraceStartLocationAndDirection(AActor* Owner, FVector& OutStartLocation, FVector& OutEndLocation, FRotator& OutDirection)
{
	// If TraceStartLocation is Camera, adjust the start location and direction based on Hero's camera
	if (TraceStartLocation == ETraceStartLocation::Camera)
	{
		if (AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(Owner))
		{
			OutStartLocation = HeroBase->GetFollowCamera()->GetComponentLocation() +
				HeroBase->GetFollowCamera()->GetForwardVector() * StartLocationForwardOffset;

			if (TraceDirectionType == ETraceDirectionType::ForwardDirection)
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

		if (TraceDirectionType == ETraceDirectionType::ForwardDirection)
		{
			OutDirection = Owner->GetActorForwardVector().Rotation();
		}
	}
}

void UGAS_TraceBase::MakeTeamFilter(TArray<AActor*>& OutActors, const AActor& Owner, ETeamAttitude::Type TeamAttidue)
{
	TArray<AActor*> FilteredActors;

	for (AActor* CollectedActor : OutActors)
	{
		if (UAC_Team* TeamComp = CollectedActor->GetComponentByClass<UAC_Team>())
		{
			if (TeamComp->GetTeamAttitudeTowards(Owner) == TeamAttidue)
			{
				FilteredActors.Add(CollectedActor);
			}
		}
	}

	OutActors = FilteredActors;
}

void UGAS_TraceBase::MakeTeamFilter(TArray<FHitResult>& OutHitResults, const AActor& Owner, ETeamAttitude::Type TeamAttidue)
{
	for (int32 i = OutHitResults.Num() - 1; i >= 0; --i)
	{
		FHitResult& CollectedResult = OutHitResults[i];
		if (AActor* HitActor = CollectedResult.GetActor())
		{
			if (UAC_Team* TeamComp = HitActor->GetComponentByClass<UAC_Team>())
			{
				if (TeamComp->GetTeamAttitudeTowards(Owner) != TeamAttidue)
				{
					OutHitResults.RemoveAt(i);
				}
			}
			else
			{
				OutHitResults.RemoveAt(i);
			}
		}
	}
}

void UGAS_TraceBase::MakeTrace(const UObject* Owner, const UWorld* World, const FTraceRequest& TraceRequest, TArray<AActor*>& OutActors)
{
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner can't be null! (This message comes from UGAS_TraceBase)"));
		return;
	}

	Initialize(Owner, TraceRequest.Direction);

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
	TraceLogic(World, TraceRequest, QueryParams, ResponseParams, HitResults);

#if WITH_EDITOR
	if(bDrawEnable)
		DrawDebugShape(World, TraceRequest);
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

void UGAS_TraceBase::MakeTrace(const UObject* Owner, const UWorld* World, const FTraceRequest& TraceRequest, TArray<FHitResult>& OutHitResults)
{
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner can't be null! (This message comes from UGAS_TraceBase)"));
		return;
	}

	Initialize(Owner, TraceRequest.Direction);

	FCollisionQueryParams QueryParams;
	if (bIgnoreSelf)
	{
		QueryParams.AddIgnoredActor(OwnerActor);
	}

	FCollisionResponseContainer ResponseContainer;
	ResponseContainer.SetAllChannels(CollisionResponse);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse = ResponseContainer;

	TraceLogic(World, TraceRequest, QueryParams, ResponseParams, OutHitResults);

#if WITH_EDITOR
	if (bDrawEnable)
		DrawDebugShape(World, TraceRequest);
#endif // WITH_EDITOR
}

void UGAS_TraceBase::Initialize(const UObject* Owner, FRotator Direction)
{
	OwnerActor = Cast<AActor>(Owner);
}

void UGAS_TraceBase::TraceLogic(const UWorld* World, const FTraceRequest& TraceRequest, const FCollisionQueryParams& QueryParams, const FCollisionResponseParams& ResponseParams, TArray<FHitResult>& OutHitResults)
{
	World->SweepMultiByChannel(
		OutHitResults,
		TraceRequest.StartLocation,
		TraceRequest.EndLocation,
		TraceRequest.Direction.Quaternion(),
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
void UGAS_TraceBase::DrawDebugShape(const UWorld* World, const FTraceRequest& TraceRequest) const
{
	// Logic will be implmeneted in subclasses.
}
#endif // WITH_EDITOR

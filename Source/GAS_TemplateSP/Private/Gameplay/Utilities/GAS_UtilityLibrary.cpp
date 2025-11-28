// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Utilities/GAS_UtilityLibrary.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

AActor* UGAS_UtilityLibrary::FindNearestActor(AActor* ReferanceActor, TArray<AActor*> ActorArray)
{
	if (ActorArray.IsEmpty())
	{
		return nullptr;
	}

	AActor* NearestActor = nullptr;
	float NearestDistance = FLT_MAX;

	for (AActor* Actor : ActorArray)
	{
		float Distance = FVector::Dist(ReferanceActor->GetActorLocation(), Actor->GetActorLocation());

		if (Distance < NearestDistance)
		{
			NearestDistance = Distance;
			NearestActor = Actor;
		}
	}

	return NearestActor;
}

void UGAS_UtilityLibrary::FilterOutDeadActors(TArray<AActor*>& Actors)
{
	// We'll build a new array of only alive actors
	TArray<AActor*> FilteredActors;

	for (AActor* Actor : Actors)
	{
		if (!Actor)
		{
			continue;
		}

		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
		if (!ASC)
		{
			continue;
		}

		if (!ASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead))
		{
			// Only keep alive actors
			FilteredActors.Add(Actor);
		}
	}

	// Replace old array with filtered version
	Actors = MoveTemp(FilteredActors);
}
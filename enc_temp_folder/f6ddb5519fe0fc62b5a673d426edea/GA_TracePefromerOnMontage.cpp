// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_TracePefromerOnMontage.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Actors/Weapons/WeaponBase.h"

void UGA_TracePefromerOnMontage::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CharacterBase) 
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}

	CharacterWeapon = CharacterBase->GetWeapon();
	if (!CharacterWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterWeapon is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}
}

void UGA_TracePefromerOnMontage::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnEventReceived(EventTag, EventData);

	if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotifyState_AttackTrace_Start)
	{
		bIsFirstTraceTick = true;
		HitActorsThisSwing.Empty();

		FVector Start, End;
		FRotator Rot;
		GetTracePoints(Start, End, Rot);

		PrevTraceStart = Start;
		PrevTraceEnd = End;
	}
	else if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotifyState_AttackTrace_Continue)
	{
		PerformInterpolatedTrace();
	}
	else if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotifyState_AttackTrace_End)
	{
		bIsFirstTraceTick = true;
		HitActorsThisSwing.Empty();
	}
}

void UGA_TracePefromerOnMontage::GetTracePoints(FVector& OutStart, FVector& OutEnd, FRotator& OutRot)
{
	if (CharacterWeapon)
	{
		OutStart = CharacterWeapon->GetTraceStart();
		OutEnd = CharacterWeapon->GetTraceEnd();
		OutRot = CharacterWeapon->GetTraceEndRotation();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetTracePoints: CharacterWeapon is null. Override this function for bone-based trace!"));
		OutStart = FVector::ZeroVector;
		OutEnd = FVector::ZeroVector;
		OutRot = FRotator::ZeroRotator;
	}
}

void UGA_TracePefromerOnMontage::PerformInterpolatedTrace()
{
	if (!TraceData || !TraceData->Trace)
	{
		return;
	}

	// Þimdiki pozisyonlarý al (override edilmiþ GetTracePoints kullanýlýr)
	FVector CurrStart, CurrEnd;
	FRotator Rot;
	GetTracePoints(CurrStart, CurrEnd, Rot);

	// Ýlk tick'te sadece pozisyonu kaydet
	if (bIsFirstTraceTick)
	{
		PrevTraceStart = CurrStart;
		PrevTraceEnd = CurrEnd;
		bIsFirstTraceTick = false;
		return;
	}

	// Hareket mesafesini hesapla
	const float DistanceMoved = FVector::Distance(PrevTraceEnd, CurrEnd);

	// Çok az hareket varsa trace yapma
	if (DistanceMoved < MinDistanceThreshold)
	{
		return;
	}

	// Interpolated trace sayýsýný hesapla
	const int32 NumSteps = FMath::Max(1, FMath::CeilToInt(DistanceMoved / MaxStepSize));

	// Önceki ve þimdiki pozisyon arasýnda interpolate ederek trace yap
	for (int32 i = 0; i <= NumSteps; i++)
	{
		const float Alpha = (float)i / (float)NumSteps;

		FVector InterpStart = FMath::Lerp(PrevTraceStart, CurrStart, Alpha);
		FVector InterpEnd = FMath::Lerp(PrevTraceEnd, CurrEnd, Alpha);

		// Bu ara pozisyonda trace yap
		TArray<FHitResult> HitResults;
		if (TraceForHostileUnits(InterpStart, InterpEnd, HitResults))
		{
			// Hit'leri filtrele - ayný actor'a bu swing'de birden fazla hasar verme
			TArray<FHitResult> FilteredHits;
			for (const FHitResult& Hit : HitResults)
			{
				AActor* HitActor = Hit.GetActor();
				if (HitActor && !HitActorsThisSwing.Contains(HitActor))
				{
					FilteredHits.Add(Hit);
					HitActorsThisSwing.Add(HitActor);
				}
			}

			if (FilteredHits.Num() > 0)
			{
				OnTraceHitResults(FilteredHits);
			}
		}
	}

	// Þimdiki pozisyonu kaydet
	PrevTraceStart = CurrStart;
	PrevTraceEnd = CurrEnd;
}

bool UGA_TracePefromerOnMontage::TraceForHostileUnits(const FVector& Start, const FVector& End, TArray<FHitResult>& OutHitResults)
{
	if (!TraceData || !TraceData->Trace)
	{
		return false;
	}

	FTraceRequest TraceRequest;
	TraceRequest.StartLocation = Start;
	TraceRequest.EndLocation = End;
	TraceRequest.Direction = (End - Start).Rotation();

	TraceData->Trace->CreateTraceWithTeamFilter(
		GetWorld(),
		GetAvatarActorFromActorInfo(),
		ETeamAttitude::Hostile,
		OutHitResults,
		TraceRequest
	);

	return OutHitResults.Num() > 0;
}

void UGA_TracePefromerOnMontage::OnTraceHitResults(const TArray<FHitResult>& HitResults)
{
	// Implementation will be in subclasses
}

void UGA_TracePefromerOnMontage::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	// Cleanup
	bIsFirstTraceTick = true;
	HitActorsThisSwing.Empty();

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


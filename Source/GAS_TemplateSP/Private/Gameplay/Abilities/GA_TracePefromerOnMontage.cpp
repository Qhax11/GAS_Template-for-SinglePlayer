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

	CharacterBase = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo());
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

	if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_Attack_TraceStart)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_TraceTick, this, &UGA_TracePefromerOnMontage::TraceTick, TraceTickValue, true, 0);
	}
	else if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_Attack_TraceEnd)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_TraceTick);
	}
}

void UGA_TracePefromerOnMontage::TraceTick()
{
	TArray<FHitResult> OutHitResults;
	if (TraceForHostileUnits(OutHitResults))
	{
		OnTraceHitResults(OutHitResults);

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_TraceTick);
	}
}

bool UGA_TracePefromerOnMontage::TraceForHostileUnits(TArray<FHitResult>& OutHitResults)
{
	if (!TraceData)
	{
		UE_LOG(LogTemp, Warning, TEXT("TraceData is null in: %s"), *GetName());
		return false;
	}

	FTraceRequest TraceRequest;
	TraceRequest.StartLocation = CharacterWeapon->GetTraceStart();
	TraceRequest.EndLocation = CharacterWeapon->GetTraceEnd();
	TraceRequest.Direction = CharacterWeapon->GetTraceEndRotation();
	TraceData->Trace->CreateTraceWithTeamFilter(GetWorld(), GetAvatarActorFromActorInfo(), ETeamAttitude::Hostile, OutHitResults, TraceRequest);

	return OutHitResults.IsValidIndex(0);
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
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


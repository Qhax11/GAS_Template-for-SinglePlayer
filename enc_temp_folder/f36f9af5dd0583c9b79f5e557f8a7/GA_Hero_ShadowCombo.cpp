// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/InCombat/GA_Hero_ShadowCombo.h"
#include "Gameplay/Abilities/DataTypes/Combo/ComboPreActivationData.h"
#include "Gameplay/Components/AC_MeleeComboManager.h"

UGA_Hero_ShadowCombo::UGA_Hero_ShadowCombo()
{
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_ShadowCombo;
	AbilityTriggers.Add(TriggerData);

	WarpTargetMode = EWarpTargetMode::PreActivation;
}

void UGA_Hero_ShadowCombo::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	if (!TriggerEventData || !TriggerEventData->OptionalObject)
	{
		UE_LOG(LogTemp, Error,
			TEXT("GA_Hero_ShadowCombo: TriggerEventData is REQUIRED. Cancelling ability."));

		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const UComboPreActivationData* PreActivationData = Cast<UComboPreActivationData>(TriggerEventData->OptionalObject);

	if (!PreActivationData)
	{
		UE_LOG(LogTemp, Error,
			TEXT("GA_Hero_ShadowCombo: Invalid PreActivationData type. Cancelling ability."));

		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	WarpTargetMode = EWarpTargetMode::PreActivation;

	SetPreActivationWarpTarget(
		PreActivationData->MotionWarpingLocation,
		PreActivationData->MotionWarpingRotation
	);

	UE_LOG(LogTemp, Log,
		TEXT("GA_Hero_ShadowCombo: PreActivation OK | Loc=%s Rot=%s"),
		*PreActivationData->MotionWarpingLocation.ToString(),
		*PreActivationData->MotionWarpingRotation.ToString());
}

void UGA_Hero_ShadowCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
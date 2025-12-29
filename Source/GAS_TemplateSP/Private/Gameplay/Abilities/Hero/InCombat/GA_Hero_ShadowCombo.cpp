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
}

void UGA_Hero_ShadowCombo::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	// ? ASIL ÇÖZÜM: Instance oluþtuðunda hemen data'yý event'ten al
	if (TriggerEventData && TriggerEventData->OptionalObject)
	{
		const UComboPreActivationData* PreActivationData = Cast<UComboPreActivationData>(TriggerEventData->OptionalObject);
		if (PreActivationData)
		{
			SetPreActivationWarpTarget(
				PreActivationData->MotionWarpingLocation,
				PreActivationData->MotionWarpingRotation
			);

			UE_LOG(LogTemp, Log, TEXT("GA_ComboMeleeAttack: ? PreActivation set from event - Loc: %s, Rot: %s"),
				*PreActivationData->MotionWarpingLocation.ToString(),
				*PreActivationData->MotionWarpingRotation.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GA_ComboMeleeAttack: ? Failed to cast PreActivationData!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GA_ComboMeleeAttack: ? TriggerEventData or OptionalObject is NULL!"));
	}

	// Debug - ability instance'a ait deðerleri kontrol et
	UE_LOG(LogTemp, Log, TEXT("GA_ComboMeleeAttack::ActivateAbility - Instance: %s, WarpLoc: %s, WarpRot: %s, Mode: %d"),
		*GetName(),
		*PreActivationWarpLocation.ToString(),
		*PreActivationWarpRotation.ToString(),
		(int32)WarpTargetMode);
}

void UGA_Hero_ShadowCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
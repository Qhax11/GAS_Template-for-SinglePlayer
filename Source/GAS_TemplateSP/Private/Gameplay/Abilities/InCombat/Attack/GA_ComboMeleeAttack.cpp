// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/InCombat/Attack/GA_ComboMeleeAttack.h"
#include "Gameplay/Abilities/DataTypes/Combo/ComboPreActivationData.h"
#include "Gameplay/Components/AC_MeleeComboManager.h"

UGA_ComboMeleeAttack::UGA_ComboMeleeAttack()
{
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_MeleeCombo);
	SetAssetTags(AssetTags);

	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_ComboMelee;
	AbilityTriggers.Add(TriggerData);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo);
}

void UGA_ComboMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	const UComboPreActivationData* PreActivationData = Cast<UComboPreActivationData>(TriggerEventData->OptionalObject);
	if (PreActivationData) 
	{
		SetPreActivationWarpTarget(PreActivationData->MotionWarpingLocation, PreActivationData->MotionWarpingRotation);
	}

	// ? manager’a "ben buyum" diye bildir
	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		if (UAC_MeleeComboManager* Manager = ActorInfo->AvatarActor->FindComponentByClass<UAC_MeleeComboManager>())
		{
			Manager->OnComboAbilityActivated(this); // bunu yaz
		}
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_ComboMeleeAttack::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnEventReceived(EventTag, EventData);
}




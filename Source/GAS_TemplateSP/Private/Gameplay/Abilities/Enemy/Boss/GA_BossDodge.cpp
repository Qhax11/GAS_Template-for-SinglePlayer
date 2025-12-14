// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Boss/GA_BossDodge.h"

UGA_BossDodge::UGA_BossDodge()
{
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_Dash;

	AbilityTriggers.Add(TriggerData);
}

void UGA_BossDodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!TriggerEventData)
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerEventData is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	if (TriggerEventData->InstigatorTags.IsValidIndex(0)) 
	{
		DirectionTag = TriggerEventData->InstigatorTags.GetByIndex(0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no direction tag in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	UAnimMontage* FoundDodgeMontage = DirectionToDodgeMontageAsset->FindDodgetMontage(DirectionTag);
	if (!FoundDodgeMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("No montage found for direction tag: %s"), *DirectionTag.ToString());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("montage found for direction tag: %s"), *DirectionTag.ToString());

	AnimMontage = FoundDodgeMontage;

	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_DamageImmune);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

FVector UGA_BossDodge::CalculateMotionWarpingLocation() const
{
	return Super::CalculateMotionWarpingLocation();
}



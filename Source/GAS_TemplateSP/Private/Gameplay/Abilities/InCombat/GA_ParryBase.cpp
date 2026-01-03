// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/InCombat/GA_ParryBase.h"
#include "Gameplay/Abilities/InCombat/GA_ParryKnockbackBase.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"


UGA_ParryBase::UGA_ParryBase()
{
	FGameplayTagContainer AbiltiyTags;
	AbiltiyTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Parry);
	SetAssetTags(AbiltiyTags);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Parry);
	ActivationOwnedTags.RemoveTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Parry);

	WarpTargetMode = EWarpTargetMode::None;
}

bool UGA_ParryBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return false;
	}

	if (UAS_Base* BaseAttributes = const_cast<UAS_Base*>(ASC->GetSet<UAS_Base>()))
	{
		return BaseAttributes->GetPosture() > GetCost(GetAbilityLevel());
	}

	return false;
}

void UGA_ParryBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_ParryBase: CharacterBase is null in: %s, can not initialize"), *GetName());
		return;
	}

	TargetCharacterTagDelegatesComp = CharacterBase->GetTagDelegatesComponent();
	if (!TargetCharacterTagDelegatesComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_ParryBase: TargetCharacterTagDelegatesComp is null in: %s, can not initialize"), *GetName());
		return;
	}

	TargetCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_Attribute_Posture_Empty, EListenMode::OnAdded).BindDynamic(this, &UGA_ParryBase::OnPostureEmptyTagAdded);
}

void UGA_ParryBase::OnParryKnocbackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	//EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_ParryBase::OnPostureEmptyTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_ParryBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	TargetCharacterTagDelegatesComp->UnregisterAllDelegatesForObject(this);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}



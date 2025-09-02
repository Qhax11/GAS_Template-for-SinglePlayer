// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_ParryBase.h"
#include "Gameplay/Abilities/GA_ParryKnockbackBase.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"

UGA_ParryBase::UGA_ParryBase()
{
	FGameplayTagContainer AbiltiyTags;
	AbiltiyTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Parry);
	SetAssetTags(AbiltiyTags);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Parry);
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
		return BaseAttributes->GetPosture() > 0.f;
	}

	return false;
}

void UGA_ParryBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	US_DamageDelegates* DamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_DamageDelegates>();
	if (!DamageSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageSubsystem is null in: %s, can not initialize"), *GetName());
		return;
	}

	if (!DamageSubsystem->OnDamageDealt.IsAlreadyBound(this, &UGA_ParryBase::OnDamageDealt))
	{
		DamageSubsystem->OnDamageDealt.AddDynamic(this, &UGA_ParryBase::OnDamageDealt);
	}

	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s, can not initialize"), *GetName());
		return;
	}

	UAC_TagDelegates* TargetCharacterTagDelegatesComp = CharacterBase->GetTagDelegatesComponent();
	if (!TargetCharacterTagDelegatesComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetCharacterTagDelegatesComp is null in: %s, can not initialize"), *GetName());
		return;
	}

	TargetCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_Attribute_Posture_Empty, EListenMode::OnAdded).BindDynamic(this, &UGA_ParryBase::OnPostureEmptyTagAdded);
}

void UGA_ParryBase::OnDamageDealt(const FDamageData& DamageData)
{
	if (!DamageData.bParrySucces)
	{
		return;
	}

	FGameplayEventData Payload;
	Payload.EventTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_ParryKnockback;
	Payload.Instigator = DamageData.ExecCalculationParameters.SourceActor;
	Payload.Target = DamageData.ExecCalculationParameters.TargetActor;
	Payload.ContextHandle = DamageData.ExecCalculationParameters.GetSpec().GetContext();
	Payload.InstigatorTags = DamageData.ExecCalculationParameters.GetSpec().CapturedSourceTags.GetActorTags();

	if (UGAS_AbilitySystemComponent* HeroASC = GetASC())
	{
		UGAS_GameplayAbilityBase* ActivatedAbility = HeroASC->TryActivateAbilityByClassWithEventData(ParryKnockbackAbilityClass, Payload);
		if (ActivatedAbility)
		{
			if (!ActivatedAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UGA_ParryBase::OnParryKnocbackAbilityEnded))
			{
				ActivatedAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UGA_ParryBase::OnParryKnocbackAbilityEnded);
			}
		}
	}
}

void UGA_ParryBase::OnParryKnocbackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_ParryBase::OnPostureEmptyTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_ParryBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (US_DamageDelegates* DamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_DamageDelegates>())
	{
		DamageSubsystem->OnDamageDealt.RemoveDynamic(this, &UGA_ParryBase::OnDamageDealt);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}



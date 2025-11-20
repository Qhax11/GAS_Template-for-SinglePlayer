// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Movement/GA_HeroRun.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"

UGA_HeroRun::UGA_HeroRun()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Moving_Running);
}

bool UGA_HeroRun::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
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
		return BaseAttributes->GetPosture() > 0;
	}

	return false;
}

void UGA_HeroRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Hero = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
	if (!Hero)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hero is null in %s, cannot damage apply"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	HeroTagDelegatesComp = Hero->GetTagDelegatesComponent();
	if (!HeroTagDelegatesComp) 
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroTagDelegatesComp is null in %s, cannot damage apply"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	FGameplayEffectSpec PostureDecreaseSpec;
	bool bIsPostureRegenSpecValid = UGAS_EffectBlueprintFunctionLibary::CreateEffectSpecWithSetByCallerValue(
		PostureDecreaseSpec,
		GetAbilitySystemComponentFromActorInfo(),
		GE_PostureDecreaseClass,
		GAS_Tags::TAG_Gameplay_EffectData_SetByCaller_PostureDecreaseAmount,
		PostureDecreaseAmount
	);

	if (!bIsPostureRegenSpecValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("PostureRegenSpec is null in %s, cannot damage apply"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	HeroTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_Finisher, EListenMode::OnAdded).BindDynamic(this, &UGA_HeroRun::OnPostureEmptyTagAdded);

	UGameplayEffect* GE_SpeedBoost = UGAS_EffectBlueprintFunctionLibary::CreateEffectWithTSubclass(GE_SpeedBoostClass);
	GE_SpeedBoostHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(GE_SpeedBoost, 1, FGameplayEffectContextHandle());

	GE_PostureDecreaseHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(PostureDecreaseSpec);
}

void UGA_HeroRun::OnPostureEmptyTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_HeroRun::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (GE_SpeedBoostHandle.IsValid() && GE_PostureDecreaseHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(GE_SpeedBoostHandle);
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(GE_PostureDecreaseHandle);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("GE_SpeedBoostHandle or GE_PostureDecreaseHandle is null in %s"), *GetName());
	}

	HeroTagDelegatesComp->UnregisterAllDelegatesForObject(this);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

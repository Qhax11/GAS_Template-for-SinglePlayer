// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroParry.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Gameplay/Abilities/InCombat/GA_ParryKnockbackBase.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"


UGA_HeroParry::UGA_HeroParry()
{
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InAir);

	MontageEndPolicy = EMontageEndPolicy::Never;
}

void UGA_HeroParry::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	WaitRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	if (WaitRelease)
	{
		WaitRelease->OnRelease.AddDynamic(this, &UGA_HeroParry::OnInputReleased);
		WaitRelease->ReadyForActivation();
	}

	US_DamageDelegates* DamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_DamageDelegates>();
	if (!DamageSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_HeroParry: DamageSubsystem is null in: %s, can not initialize"), *GetName());
		return;
	}

	if (!DamageSubsystem->OnDamageDealt.IsAlreadyBound(this, &UGA_HeroParry::OnDamageDealt))
	{
		DamageSubsystem->OnDamageDealt.AddDynamic(this, &UGA_HeroParry::OnDamageDealt);
	}

	TargetCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InAir, EListenMode::OnAdded).BindDynamic(this, &UGA_HeroParry::OnHeroInAirTagAdded);
}

void UGA_HeroParry::OnDamageDealt(const FDamageData& DamageData)
{
	if (DamageData.ExecCalculationParameters.TargetActor != GetAvatarActorFromActorInfo())
	{
		return;
	}

	if (!DamageData.bParrySucces)
	{
		return;
	}

	// Ability is PerActor, so we must end the previous activation before triggering it again.
	if (ActivatedKnockbackAbility && ActivatedKnockbackAbility->IsActive())
	{
		ActivatedKnockbackAbility->EndAbilityManually();
	}

	FGameplayEventData Payload;
	Payload.EventTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_ParryKnockback;
	Payload.Instigator = DamageData.ExecCalculationParameters.SourceActor;
	Payload.Target = DamageData.ExecCalculationParameters.TargetActor;
	Payload.ContextHandle = DamageData.ExecCalculationParameters.GetSpec().GetContext();
	Payload.InstigatorTags = DamageData.ExecCalculationParameters.GetSpec().CapturedSourceTags.GetActorTags();

	ActivatedKnockbackAbility = GetASC()->TryActivateAbilityByClassWithEventData(ParryKnockbackAbilityClass, Payload);
	if (ActivatedKnockbackAbility)
	{
		ActivatedKnockbackAbility->OnAbilityEnded.RemoveAll(this);
		ActivatedKnockbackAbility->OnAbilityEnded.AddUObject(this, &UGA_HeroParry::OnParryKnocbackAbilityEnded);
	}
}

void UGA_HeroParry::OnInputReleased(float TimeHeld)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UGA_HeroParry::OnParryKnocbackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	const FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	if (!Spec || !Spec->InputPressed)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return; 
	}

	CreatePlayMontageWaitForEvent();
}

void UGA_HeroParry::OnHeroInAirTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UGA_HeroParry::OnMontageBlendOut(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Parry is using montage loop. Override parent EndAbility() execution.
}

void UGA_HeroParry::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Parry is using montage loop. Override parent EndAbility() execution.
}

void UGA_HeroParry::EndAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (WaitRelease && IsValid(WaitRelease)) 
	{
		WaitRelease->EndTask();
	}

	if (US_DamageDelegates* DamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_DamageDelegates>())
	{
		DamageSubsystem->OnDamageDealt.RemoveDynamic(this, &UGA_HeroParry::OnDamageDealt);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

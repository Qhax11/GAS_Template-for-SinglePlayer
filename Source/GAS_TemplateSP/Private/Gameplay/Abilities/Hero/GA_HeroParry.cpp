// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroParry.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Gameplay/Abilities/InCombat/GA_ParryKnockbackBase.h"

UGA_HeroParry::UGA_HeroParry()
{
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InAir);
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
		UE_LOG(LogTemp, Warning, TEXT("DamageSubsystem is null in: %s, can not initialize"), *GetName());
		return;
	}

	if (!DamageSubsystem->OnDamageDealt.IsAlreadyBound(this, &UGA_HeroParry::OnDamageDealt))
	{
		DamageSubsystem->OnDamageDealt.AddDynamic(this, &UGA_HeroParry::OnDamageDealt);
	}
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
			ActivatedAbility->OnAbilityEnded.RemoveAll(this);
			ActivatedAbility->OnAbilityEnded.AddUObject(this, &UGA_HeroParry::OnParryKnocbackAbilityEnded);
		}
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

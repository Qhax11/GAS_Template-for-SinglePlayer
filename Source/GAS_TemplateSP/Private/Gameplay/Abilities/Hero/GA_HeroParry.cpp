// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroParry.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/RootMotionSource.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Abilities/Tasks/GAS_Task_PlayMontageWaitForEvent.h"

void UGA_HeroParry::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (US_DamageDelegates* DamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_DamageDelegates>())
	{
		DamageSubsystem->OnDamageDealt.AddDynamic(this, &UGA_HeroParry::OnDamageDealt);
	}

	AGAS_HeroBase* Hero = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
	if (!Hero)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hero is null in: %s, can not initialize"), *GetName());
		return;
	}

	UAC_TagDelegates* TargetCharacterTagDelegatesComp = Hero->GetTagDelegatesComponent();
	if (!TargetCharacterTagDelegatesComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hero is null in: %s, can not initialize"), *GetName());
		return;
	}

	TargetCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_Attribute_Posture_Empty, EListenMode::OnAdded).BindDynamic(this, &UGA_HeroParry::OnPostureEmptyTagAdded);
}

void UGA_HeroParry::OnMontageBlendOut(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Overridden so that when the knockback montage is played/interrupted, 
	// the ability does NOT end. Prevents automatic ending of the ability on interruption.
}

void UGA_HeroParry::OnMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Overridden so that when the knockback montage is played/interrupted, 
    // the ability does NOT end. Prevents automatic ending of the ability on interruption.
}

void UGA_HeroParry::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Overridden so that when the knockback montage is played/interrupted, 
	// the ability does NOT end. Prevents automatic ending of the ability on interruption.
}

void UGA_HeroParry::OnPostureEmptyTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_HeroParry::OnKnocbackMontageMontageBlendOut(FGameplayTag EventTag, FGameplayEventData EventData)
{
	CreatePlayMontageWaitForEvent();
}

void UGA_HeroParry::OnKnocbackMontageMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	CreatePlayMontageWaitForEvent();
}

void UGA_HeroParry::OnKnocbackMontageMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	CreatePlayMontageWaitForEvent();
}

void UGA_HeroParry::OnKnocbackMontageMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	CreatePlayMontageWaitForEvent();
}

void UGA_HeroParry::OnKnocbackMontageMontageEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == GAS_Tags::TAG_Gameplay_AnimNotify_Event_MotionWarping)
	{
		if (bEnableMotionWarping)
		{
			ActivateMotionWarping();
		}
	}
}

void UGA_HeroParry::OnDamageDealt(const FDamageData& DamageData)
{
	// Check if the ability is active and valid
	if (!IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability is not active!"));
		return;
	}

	PlayMontageKnocback = UGAS_Task_PlayMontageWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, KnocbackMontage, WaitForEventTag, PlayRate, SectionName, bStopWhenAbilityEnds, 1.0f);
	PlayMontageKnocback->OnBlendOut.AddDynamic(this, &UGA_HeroParry::OnKnocbackMontageMontageBlendOut);
	PlayMontageKnocback->OnCompleted.AddDynamic(this, &UGA_HeroParry::OnKnocbackMontageMontageCompleted);
	PlayMontageKnocback->OnInterrupted.AddDynamic(this, &UGA_HeroParry::OnKnocbackMontageMontageInterrupted);
	PlayMontageKnocback->OnCancelled.AddDynamic(this, &UGA_HeroParry::OnKnocbackMontageMontageCancelled);
	PlayMontageKnocback->EventReceived.AddDynamic(this, &UGA_HeroParry::OnKnocbackMontageMontageEventReceived);
	PlayMontageKnocback->ReadyForActivation();
	
	if (!ParryKnockbackEffect)
	{
		return;
	}

	FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(ParryKnockbackEffect, 1.0f, DamageData.ExecCalculationParameters.GetSpec().GetContext());
	if (!EffectSpecHandle.IsValid())
	{
		return;
	}

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);

	/*

	// Direction to push — here we're pushing the owner backward
	FVector PushDirection = -GetAvatarActorFromActorInfo()->GetActorForwardVector();
	float Strength = 800.f;
	float Duration = 0.4f;
	bool bIsAdditive = true;
	bool bDisableCollision = false;
	ERootMotionFinishVelocityMode VelocityMode = ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity;

	// Create the task
	UAbilityTask_ApplyRootMotionConstantForce* RootMotionTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
		this,                           // Owning Ability
		FName("ParryPushback"),         // Task Instance Name
		PushDirection,                  // Direction
		Strength,                       // Strength
		Duration,                       // Duration
		false,                          // bIsAdditive
		CurveFloat,                     // StrengthOverTime (optional)
		ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity, // VelocityMode
		FVector(0,0,0),
		0.0f,                           // SetVelocityOnFinish
		false                          // bEnableGravity
	);

	if (RootMotionTask)
	{
		RootMotionTask->ReadyForActivation();
	}
	*/
	BPOnDamageDealt(DamageData);
}

void UGA_HeroParry::EndAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (US_DamageDelegates* DamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_DamageDelegates>())
	{
		DamageSubsystem->OnDamageDealt.RemoveDynamic(this, &UGA_HeroParry::OnDamageDealt);
	}

	if (PlayMontageKnocback)
	{
		PlayMontageKnocback->StopPlayingMontage();
		PlayMontageKnocback->EndTask();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

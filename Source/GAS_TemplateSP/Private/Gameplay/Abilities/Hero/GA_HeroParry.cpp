// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroParry.h"
#include <Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h>
#include "GameFramework/RootMotionSource.h"

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
}

void UGA_HeroParry::OnMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	
}

void UGA_HeroParry::OnKnocbackMontageMontageBlendOut(FGameplayTag EventTag, FGameplayEventData EventData)
{
	FScriptDelegate BlendOutDel;
	BlendOutDel.BindUFunction(this, FName("OnMontageBlendOut"));
	FScriptDelegate CompletedDel;
	CompletedDel.BindUFunction(this, FName("OnMontageCompleted"));
	FScriptDelegate InterruptedDel;
	InterruptedDel.BindUFunction(this, FName("OnMontageInterrupted"));
	FScriptDelegate CancelledDel;
	CancelledDel.BindUFunction(this, FName("OnMontageCancelled"));
	FScriptDelegate EventReceivedDel;
	EventReceivedDel.BindUFunction(this, FName("OnEventReceived"));
	CreatePlayMontageWaitForEvent(NAME_None, AnimMontage, WaitForEventTag, PlayRate, SectionName, bStopWhenAbilityEnds, 1.0f, BlendOutDel, CompletedDel, InterruptedDel, CancelledDel, EventReceivedDel);
}

void UGA_HeroParry::OnKnocbackMontageMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	FScriptDelegate BlendOutDel;
	BlendOutDel.BindUFunction(this, FName("OnMontageBlendOut"));
	FScriptDelegate CompletedDel;
	CompletedDel.BindUFunction(this, FName("OnMontageCompleted"));
	FScriptDelegate InterruptedDel;
	InterruptedDel.BindUFunction(this, FName("OnMontageInterrupted"));
	FScriptDelegate CancelledDel;
	CancelledDel.BindUFunction(this, FName("OnMontageCancelled"));
	FScriptDelegate EventReceivedDel;
	EventReceivedDel.BindUFunction(this, FName("OnEventReceived"));
	CreatePlayMontageWaitForEvent(NAME_None, AnimMontage, WaitForEventTag, PlayRate, SectionName, bStopWhenAbilityEnds, 1.0f, BlendOutDel, CompletedDel, InterruptedDel, CancelledDel, EventReceivedDel);
}

void UGA_HeroParry::OnKnocbackMontageMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	FScriptDelegate BlendOutDel;
	BlendOutDel.BindUFunction(this, FName("OnMontageBlendOut"));
	FScriptDelegate CompletedDel;
	CompletedDel.BindUFunction(this, FName("OnMontageCompleted"));
	FScriptDelegate InterruptedDel;
	InterruptedDel.BindUFunction(this, FName("OnMontageInterrupted"));
	FScriptDelegate CancelledDel;
	CancelledDel.BindUFunction(this, FName("OnMontageCancelled"));
	FScriptDelegate EventReceivedDel;
	EventReceivedDel.BindUFunction(this, FName("OnEventReceived"));
	CreatePlayMontageWaitForEvent(NAME_None, AnimMontage, WaitForEventTag, PlayRate, SectionName, bStopWhenAbilityEnds, 1.0f, BlendOutDel, CompletedDel, InterruptedDel, CancelledDel, EventReceivedDel);
}

void UGA_HeroParry::OnKnocbackMontageMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	FScriptDelegate BlendOutDel;
	BlendOutDel.BindUFunction(this, FName("OnMontageBlendOut"));
	FScriptDelegate CompletedDel;
	CompletedDel.BindUFunction(this, FName("OnMontageCompleted"));
	FScriptDelegate InterruptedDel;
	InterruptedDel.BindUFunction(this, FName("OnMontageInterrupted"));
	FScriptDelegate CancelledDel;
	CancelledDel.BindUFunction(this, FName("OnMontageCancelled"));
	FScriptDelegate EventReceivedDel;
	EventReceivedDel.BindUFunction(this, FName("OnEventReceived"));
	CreatePlayMontageWaitForEvent(NAME_None, AnimMontage, WaitForEventTag, PlayRate, SectionName, bStopWhenAbilityEnds, 1.0f, BlendOutDel, CompletedDel, InterruptedDel, CancelledDel, EventReceivedDel);
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
	UE_LOG(LogTemp, Warning, TEXT("OnDamageDealt!"));

	// Check if the ability is active and valid
	if (!IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability is not active!"));
		return;
	}


	FScriptDelegate BlendOutDel;
	BlendOutDel.BindUFunction(this, FName("OnKnocbackMontageMontageBlendOut"));
	FScriptDelegate CompletedDel;
	CompletedDel.BindUFunction(this, FName("OnKnocbackMontageMontageCompleted"));
	FScriptDelegate InterruptedDel;
	InterruptedDel.BindUFunction(this, FName("OnKnocbackMontageMontageInterrupted"));
	FScriptDelegate CancelledDel;
	CancelledDel.BindUFunction(this, FName("OnKnocbackMontageMontageCancelled"));
	FScriptDelegate EventReceivedDel;
	EventReceivedDel.BindUFunction(this, FName("OnKnocbackMontageMontageEventReceived"));
	CreatePlayMontageWaitForEvent(NAME_None, KnocbackMontage, WaitForEventTag, PlayRate, SectionName, bStopWhenAbilityEnds, 1.0f, BlendOutDel, CompletedDel, InterruptedDel, CancelledDel, EventReceivedDel);
	
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
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

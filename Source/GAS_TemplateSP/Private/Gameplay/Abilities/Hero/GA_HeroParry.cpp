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

void UGA_HeroParry::OnDamageDealt(const FDamageData& DamageData)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDamageDealt!"));

	// Check if the ability is active and valid
	if (!IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability is not active!"));
		return;
	}

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

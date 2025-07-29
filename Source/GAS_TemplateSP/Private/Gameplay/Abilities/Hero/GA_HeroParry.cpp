// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroParry.h"

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

void UGA_HeroParry::OnDamageDealt(const FDamageData& DamageData)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDamageDealted!"));
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

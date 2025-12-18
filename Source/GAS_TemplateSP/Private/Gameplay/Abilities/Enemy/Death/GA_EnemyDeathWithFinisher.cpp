// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Death/GA_EnemyDeathWithFinisher.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeFinisher.h"

UGA_EnemyDeathWithFinisher::UGA_EnemyDeathWithFinisher()
{
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_Death_Finisher;
	AbilityTriggers.Add(TriggerData);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Finisher);
}

void UGA_EnemyDeathWithFinisher::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	if (!TriggerEventData) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyDeathWithFinisher: TriggerEventData is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Retrieve the instigator ability from OptionalObject, called from shadowfinisher
	const UGA_MeleeFinisher* MeleeFinisher = Cast<UGA_MeleeFinisher>(TriggerEventData->ContextHandle.GetAbility());
	if (!MeleeFinisher)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyDeathWithFinisher: MeleeFinisher is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FGameplayTag FinisherTypeTag = MeleeFinisher->GetFinisherTypeTagFromAbilityTags();

	if (!FinisherHitData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyDeathWithFinisher: FinisherHitData is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAnimMontage* FinisherHitMontage = FinisherHitData->FindMontageByTag(FinisherTypeTag);
	if (!FinisherHitMontage) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyDeathWithFinisher: FinisherHitMontage is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AnimMontage = FinisherHitMontage;
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	DisableOwnerCollision(ECollisionEnabled::QueryOnly);
}

void UGA_EnemyDeathWithFinisher::EndAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

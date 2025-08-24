// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_TakeDamageBase.h"
#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "AIController.h"

UGA_TakeDamageBase::UGA_TakeDamageBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_TakeDamage;
	AbilityTriggers.Add(TriggerData);

	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Basic);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Finisher);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_UnstoppableAttack);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage);
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_TakeDamage);
}

void UGA_TakeDamageBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	if (!TriggerEventData) 
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerEventData is null in: %s"), *GetName());
		return;
	}

	if (const UGA_MeleeAttackBase* MeleeAttackBase = Cast<UGA_MeleeAttackBase>(TriggerEventData->ContextHandle.GetAbility()))
	{
		AnimMontage = GetHitMontage(MeleeAttackBase->AnimMontage);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

UAnimMontage* UGA_TakeDamageBase::GetHitMontage(UAnimMontage* AttackMontage)
{
	if (!ReactionDataAsset) 
	{
		return nullptr;
	}

	return ReactionDataAsset->FindHitMontage(AttackMontage);
}



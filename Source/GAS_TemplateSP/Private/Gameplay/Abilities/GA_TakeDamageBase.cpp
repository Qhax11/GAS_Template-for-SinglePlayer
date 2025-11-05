// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_TakeDamageBase.h"
#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "AIController.h"

UGA_TakeDamageBase::UGA_TakeDamageBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	// Set the ability to trigger on TakeDamage gameplay event
	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_TakeDamage;
	AbilityTriggers.Add(TriggerData);

	// Block activation if the character is dead or unstoppable
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Basic);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Finisher);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_UnstoppableAttack);

	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage);
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_TakeDamage);
}

void UGA_TakeDamageBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	if (!TriggerEventData) 
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerEventData is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	// Use default montage, it should be in air attack hit.
	if(GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InAir))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetAbilitySystemComponentFromActorInfo in air"));
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		return;
	}

	const UGA_MeleeAttackBase* MeleeAttack = Cast<UGA_MeleeAttackBase>(TriggerEventData->ContextHandle.GetAbility());
	if (!MeleeAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeAttackBase is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	FGameplayTag AttackDirectionTag = MeleeAttack->GetAttackDirectionTagFromAbilityTags();
	FGameplayTag AdjustedTag = GetAdjustedAttackDirectionTag(AttackDirectionTag, TriggerEventData->Instigator);
	AnimMontage = GetHitMontage(AdjustedTag);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

FGameplayTag UGA_TakeDamageBase::GetAdjustedAttackDirectionTag(FGameplayTag InComingAttackDirection, const AActor* Instigator)
{
	if (!InComingAttackDirection.IsValid() || !Instigator)
	{
		return FGameplayTag();
	}

	// Compute vector from character to attack instigator
	FVector AttackDir = (Instigator->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal();
	FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	float Dot = FVector::DotProduct(Forward, AttackDir);

	// Character is facing away(backward)
	if (Dot < 0.f) 
	{
		if (InComingAttackDirection == GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_LeftToRight)
		{
			InComingAttackDirection = GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_RightToLeft;
		}
		else if (InComingAttackDirection == GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_RightToLeft)
		{
			InComingAttackDirection = GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_LeftToRight;
		}
		else if (InComingAttackDirection == GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_Forward)
		{
			InComingAttackDirection = GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_Backward;
		}
		else if (InComingAttackDirection == GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_Backward)
		{
			InComingAttackDirection = GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_Direction_Forward;
		}
	}

	return InComingAttackDirection;
}

UAnimMontage* UGA_TakeDamageBase::GetHitMontage(FGameplayTag InComingAttackDirectionTag)
{
	if (!ReactionDataAsset) 
	{
		return nullptr;
	}

	return ReactionDataAsset->FindHitMontage(InComingAttackDirectionTag);
}



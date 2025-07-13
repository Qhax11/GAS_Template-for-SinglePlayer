// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_DeathBase.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"

UGA_DeathBase::UGA_DeathBase()
{
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Basic);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Finisher);
}

void UGA_DeathBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!TriggerEventData || !TriggerEventData->Instigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerEventData or instigator is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	SetupBrodcastDeSpawn(TriggerEventData->Instigator);
	RemoveTags();
}

void UGA_DeathBase::SetupBrodcastDeSpawn(const AActor* Instigator)
{
	AGAS_CharacterBase* OwnerCharacter = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();

	if (!OwnerCharacter || !OwnerASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter or OwnerASC is null in: %s"), *GetName());
		return;
	}

	// Try casting the instigator without const_cast
	const AGAS_CharacterBase* InstigatorCharacterConst = Cast<AGAS_CharacterBase>(Instigator);
	AGAS_CharacterBase* InstigatorCharacter = const_cast<AGAS_CharacterBase*>(InstigatorCharacterConst); 

	UAbilitySystemComponent* InstigatorASC = InstigatorCharacter ? InstigatorCharacter->GetAbilitySystemComponent() : nullptr;

	FCharacterDeSpawnData CharacterDeSpawnData(OwnerCharacter, OwnerASC, InstigatorCharacter, InstigatorASC);
	BroadcastDeSpawn(CharacterDeSpawnData);
}

void UGA_DeathBase::RemoveTags()
{
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
	if (OwnerASC)
	{
		for (const FGameplayTag& Tag : TagsToRemove)
		{
			OwnerASC->RemoveLooseGameplayTag(Tag, 100);
		}
	}
}

void UGA_DeathBase::BroadcastDeSpawn(const FCharacterDeSpawnData& DespawnData)
{
	// Logic will be implemented in subclasses.
}

void UGA_DeathBase::EndAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AGAS_CharacterBase* CharacterBase = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo()))
	{
		CharacterBase->DisableMovement();
		CharacterBase->DisableMesh();
	}
}

void UGA_DeathBase::DisableOwnerCollision()
{
	if (AGAS_CharacterBase* CharacterBase = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo()))
	{
		CharacterBase->DisableCollision();
	}
}






// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/InCombat/GA_DeathBase.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"

UGA_DeathBase::UGA_DeathBase()
{
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Basic);
	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead_Finisher);

	ActivationBlockedTags.RemoveTag(GAS_Tags::TAG_Gameplay_State_Phase_Active);
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
	
	CachedInstigator = Cast<AGAS_CharacterBase>(TriggerEventData->Instigator);
	BrodcastDeSpawn(EDeSpawnPhase::DeathStarted);
	RemoveTags();
}

void UGA_DeathBase::BrodcastDeSpawn(EDeSpawnPhase DeSpawnPhase)
{
	AGAS_CharacterBase* OwnerCharacter = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();

	if (!OwnerCharacter || !OwnerASC || !CachedInstigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter or OwnerASC is null in: %s"), *GetName());
		return;
	}

	UAbilitySystemComponent* InstigatorASC = CachedInstigator ? CachedInstigator->GetAbilitySystemComponent() : nullptr;

	FCharacterDeSpawnData CharacterDeSpawnData(DeSpawnPhase, OwnerCharacter, OwnerASC, CachedInstigator, InstigatorASC);
	BroadcastDeSpawn(CharacterDeSpawnData);
}

void UGA_DeathBase::BroadcastDeSpawn(const FCharacterDeSpawnData& DespawnData)
{
	// Logic will be implemented in subclasses.
}

void UGA_DeathBase::DisableOwnerCollision(ECollisionEnabled::Type NewType)
{
	if (!CharacterBase) 
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
		return;
	}

	CharacterBase->DisableCollision(NewType);
}

void UGA_DeathBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
		return;
	}

	CharacterBase->DisableMovement();
	CharacterBase->DisableMesh();
	CharacterBase->DisableCollision(ECollisionEnabled::NoCollision);

	BrodcastDeSpawn(EDeSpawnPhase::DeathFinished);
}




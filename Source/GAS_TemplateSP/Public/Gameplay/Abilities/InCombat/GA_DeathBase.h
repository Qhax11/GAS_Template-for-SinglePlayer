// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "GA_DeathBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_DeathBase : public UGA_MontageAbility
{
	GENERATED_BODY()

public:
	UGA_DeathBase();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void BroadcastDeSpawn(const FCharacterDeSpawnData& DespawnData);

	void DisableOwnerCollision(ECollisionEnabled::Type NewType);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UPROPERTY(EditDefaultsOnly, Category = "DeathBase")
	FGameplayTagContainer TagsToRemove;

private:
	UPROPERTY()
	const AGAS_CharacterBase* CachedInstigator = nullptr;

	void BrodcastDeSpawn(EDeSpawnPhase DeSpawnPhase);

	void RemoveTags();
};

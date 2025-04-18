// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "S_TutorialManager.generated.h"

struct FCharacterSpawnData;

UCLASS()
class GAS_TEMPLATESP_API US_TutorialManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void OnHeroSpawn(const FHeroSpawnData& HeroSpawnData);

	UFUNCTION()
	void OnParryKnockbackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnTargetChanged(AActor* NewTarget);

	AGAS_HeroBase* Hero;
};

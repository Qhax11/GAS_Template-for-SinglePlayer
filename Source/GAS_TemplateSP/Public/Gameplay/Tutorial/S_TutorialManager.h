// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "Gameplay/Tutorial/DS_Tutorial.h"
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

	void BindAllTutorailTriggers();

	AGAS_HeroBase* Hero;
	APlayerController* HeroPC;
	UAC_TagDelegates* HeroTagDelegatesComp;
	UAC_TargetLockSystem* HeroTargetLockSystemComp;
	const UDS_Tutorial* TutorialSettings;

	UPROPERTY()
	class UW_TutorialQuest* CurrentQuestWidget = nullptr;

	UFUNCTION()
	void OnTutorailTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void OnTutorialAbilityInfoClosed(const class UW_TutorialAbilityInfo* ClosedTutorialWidget);

	UFUNCTION(BlueprintCallable)
	void OnQuestIsFinished(const class UW_TutorialQuest* FinishedQuestWidget);
};

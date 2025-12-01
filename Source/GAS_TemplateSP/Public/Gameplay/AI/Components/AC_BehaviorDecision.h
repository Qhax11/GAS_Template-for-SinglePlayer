// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReactionBase.h"
#include "Gameplay/AI/BehaviorDecision/BDS_GetBestAttack.h"
#include "Gameplay/AI/BehaviorDecision/BDS_GetBestMovementChain.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "AC_BehaviorDecision.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBehaviorDecisionInitialized);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_BehaviorDecision : public UActorComponent
{
	GENERATED_BODY()

protected:
	UAC_BehaviorDecision();

	virtual void BeginPlay() override;

    void CreateAndInitalizeServiceses();

    UPROPERTY(EditDefaultsOnly, Category = "UAC_BehaviorDecision")
    UAttackAbilityAsset* AttackAbilityAsset;

    UPROPERTY(EditDefaultsOnly, Category = "UAC_BehaviorDecision")
    UAttackAbilityMovementChainMapAsset* AttackAbilityMovementChainMapAsset;

    UPROPERTY(EditDefaultsOnly, Category = "UAC_BehaviorDecision")
    UComingAttackReactionAsset* ComingAttackReactionAsset;

    UPROPERTY(EditDefaultsOnly)
    float SecondsCheckMovement = 1.0f;

    UPROPERTY(EditDefaultsOnly)
    bool EnableSelectedDebug = false;

    UPROPERTY(EditDefaultsOnly)
    bool EnableAllDataDebug = false;

public:
    UFUNCTION(BlueprintCallable)
    FAttackData GetBestAttack();

    UFUNCTION(BlueprintCallable)
    TArray<FMovementAbilityData> GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass);

    UFUNCTION(BlueprintCallable)
    UBDS_ComingAttackReactionBase* GetBestComingAttackReaction(struct FComingAttackPayload ComingAttackPayload);

    FAttackData LastSelectedAttackAbilityData;
    UBDS_ComingAttackReactionBase* LastSelectedComingAttackReaction;

    UPROPERTY(BlueprintAssignable)
    FOnBehaviorDecisionInitialized OnBehaviorDecisionInitialized;

protected:
    UPROPERTY(EditDefaultsOnly)
    EBehaviorState BehaviorState = EBehaviorState::None;

    class AAIControllerBase* OwnerController;
    class AGAS_EnemyBase* OwnerEnemyBase;
    class UAbilitySystemComponent* OwnerEnemyASC;
    class AGAS_HeroBase* HeroBase;
    UAC_HeroMovementListener* HeroMovementListenerComp;

    UPROPERTY()
    UBDS_ComingAttackReactionBase* ComingAttackReactionService;
	TSubclassOf<UBDS_ComingAttackReactionBase> ComingAttackReactionServiceClass;

    UPROPERTY()
    UBDS_GetBestAttack* GetBestAttackService;
    TSubclassOf<UBDS_GetBestAttack> GetBestAttackServiceClass;

    UPROPERTY()
    UBDS_GetBestMovementChain* GetBestMovementChainService;
    TSubclassOf<UBDS_GetBestMovementChain> GetBestMovementChainClass;

};

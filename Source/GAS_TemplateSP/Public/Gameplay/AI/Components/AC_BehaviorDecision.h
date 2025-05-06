// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "Gameplay/AI/BehaviorDecision/BDS_ComingAttackReaction.h"
#include "Gameplay/AI/BehaviorDecision/BDS_GetBestAttack.h"
#include "Gameplay/AI/BehaviorDecision/BDS_GetBestMovementChain.h"
#include "AC_BehaviorDecision.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_BehaviorDecision : public UActorComponent
{
	GENERATED_BODY()

protected:
	UAC_BehaviorDecision();

	virtual void BeginPlay() override;

    UFUNCTION()
    void OnTargetDetected(AActor* Target);

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
    FAttackData GetBestAttack(float DistanceToTarget);

    UFUNCTION(BlueprintCallable)
    TArray<FMovementAbilityData> GetBestMovementChain(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAbilityClass);

    UFUNCTION(BlueprintCallable)
    EComingAttackReaction GetComingAttackDecision(struct FComingAttackPayload ComingAttackPayload);

    FAttackData LastSelectedAttackAbilityData;

protected:
    UPROPERTY(EditDefaultsOnly)
    EBehaviorState BehaviorState = EBehaviorState::None;

    class AAIControllerBase* OwnerController;
    class AGAS_EnemyBase* OwnerEnemyBase;
    class UAbilitySystemComponent* OwnerEnemyASC;
    class AGAS_HeroBase* HeroBase;
    UAC_HeroMovementListener* HeroMovementListenerComp;

    UBDS_ComingAttackReaction* ComingAttackReactionService;
    UBDS_GetBestAttack* GetBestAttackService;
    UBDS_GetBestMovementChain* GetBestMovementChainService;
};

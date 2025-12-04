// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "Gameplay/AI/DataTypes/AttackData.h"
#include "Gameplay/AI/BehaviorDecision/Services/BDS_GetBestMovementChain.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "AC_BehaviorDecision.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBehaviorDecisionInitialized);

class UComingAttackReactionData;
class UBDS_ComingAttackReactionBase;
class UBDS_GetBestAttack;
struct FComingAttackPayload;

UCLASS(BlueprintType)
class UBehaviorDecisionConfigAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowAbstract = "false"))
    TSubclassOf<UBDS_GetBestAttack> AttackDecisionServiceClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowAbstract = "false"))
    TSubclassOf<UBDS_GetBestMovementChain> MovementChainServiceClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowAbstract = "false"))
    TSubclassOf<UBDS_ComingAttackReactionBase> ComingAttackReactionServiceClass;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_BehaviorDecision : public UActorComponent
{
	GENERATED_BODY()

protected:
	UAC_BehaviorDecision();

	virtual void BeginPlay() override;

    UFUNCTION()
    void OnTargetDetected(AActor* DetectedTarget);

    void CreateAndInitalizeServiceses();

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
    UComingAttackReactionData* GetBestComingAttackReaction(FComingAttackPayload ComingAttackPayload);

    FAttackData LastSelectedAttackAbilityData;

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

    UPROPERTY(EditDefaultsOnly)
    UBehaviorDecisionConfigAsset* BehaviorDecisionConfigAsset;

private:
    UPROPERTY()
    UBDS_GetBestAttack* GetBestAttackService;

    UPROPERTY()
    UBDS_GetBestMovementChain* GetBestMovementChainService;

    UPROPERTY()
    UBDS_ComingAttackReactionBase* ComingAttackReactionService;
};

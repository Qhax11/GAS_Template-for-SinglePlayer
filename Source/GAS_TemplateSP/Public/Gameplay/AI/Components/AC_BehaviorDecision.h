// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/Components/AC_AIControllerBase.h"
#include "Gameplay/AI/DataTypes/Behavior/MovementChainData.h"
#include "Gameplay/AI/DataTypes/Behavior/AttackData.h"
#include "AC_BehaviorDecision.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBehaviorDecisionInitialized);

class UComingAttackReactionData;
class UBDS_ComingAttackReactionBase;
class UBDS_GetBestAttack;
class UBDS_GetBestMovementChain;
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
class GAS_TEMPLATESP_API UAC_BehaviorDecision : public UAC_AIControllerBase
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

    class UAC_HeroMovementListener* HeroMovementListenerComp;

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

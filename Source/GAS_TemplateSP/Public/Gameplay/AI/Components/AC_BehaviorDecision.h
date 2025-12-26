// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/Components/AC_AIControllerBase.h"
#include "AC_BehaviorDecision.generated.h"

class UAttackDataBase;
class UMovementDataBase;
class UComingAttackReactionData;
class UBDS_ComingAttackReactionBase;
class UBDS_GetBestAttack;
class UBDS_GetBestMovementChain;
class UAC_IntendManager;
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

    virtual void OnHeroSpawned(const FHeroSpawnData& HeroSpawnData) override;

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
    // Core Decision Functions
    UFUNCTION(BlueprintCallable, Category = "AI|Behavior Decision")
    UAttackDataBase* GetBestAttack();

    UFUNCTION(BlueprintCallable, Category = "AI|Behavior Decision")
    UMovementChainData* GetBestMovementChain(UAttackDataBase* SelectedAttackData);

    UFUNCTION(BlueprintCallable, Category = "AI|Behavior Decision")
    UComingAttackReactionData* GetBestComingAttackReaction(FComingAttackPayload ComingAttackPayload);

    // State
    UPROPERTY(BlueprintReadOnly, Category = "AI|Behavior Decision")
    UAttackDataBase* LastSelectedAttackData;

protected:
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

    UPROPERTY()
    UAC_IntendManager* IntendManager;
};

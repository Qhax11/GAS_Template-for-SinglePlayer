// Qhax's GAS Template for 2D SinglePlayer

#pragma once

#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "GAS_EnemyBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AGAS_EnemyBase : public AGAS_CharacterBase
{
	GENERATED_BODY()

public:
	AGAS_EnemyBase(const class FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AAIControllerBase* GetEnemyController() const { return EnemyController; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USceneComponent* GetFinisherPointComponent() const { return FinisherPointComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UAC_EnemyMeleeComboManager* GetEnemyMeleeComboManagerComponent() const { return EnemyMeleeComboManagerComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UAC_EnemyMovementManager* GetEnemyMovementManagerComponent() const { return EnemyMovementManagerComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UAC_PatrolHandler* GetPatrolHandlerComponent() const { return PatrolHandlerComponent; }

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void BrodcastCharacterSpawn() override;

	//* Components *//
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Components")
	class UAC_EnemyRespawn* EnemyRespawnComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Components")
	class UAC_EnemyMeleeComboManager* EnemyMeleeComboManagerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Components")
	class UAC_EnemyMovementManager* EnemyMovementManagerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Components")
	class UAC_PatrolHandler* PatrolHandlerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Components")
	class UAC_EnemyTagListener* EnemyTagListenerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Components")
    USceneComponent* FinisherPointComponent;

	class AAIControllerBase* EnemyController;
};

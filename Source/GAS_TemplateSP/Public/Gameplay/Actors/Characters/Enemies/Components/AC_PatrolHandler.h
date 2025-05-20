// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyPatrolling.h"
#include "AC_PatrolHandler.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_PatrolHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_PatrolHandler();

	UFUNCTION(BlueprintCallable)
	void StartPatrolling();

	UFUNCTION(BlueprintCallable)
	void StopPatrolling();

protected:
	virtual void BeginPlay() override;

	void MoveToNextPatrolPoint();

	void ActivatePatrollingAbility();

	UFUNCTION()
	void OnPatrollingAbilityEnded(const FAbilityEndedDataBP& ShadowAttackAbilityEndedData);

	UPROPERTY(EditAnywhere, Category = "Patrolling")
	TArray<AActor*> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "Patrolling")
	TSubclassOf<UGA_EnemyPatrolling> EnemyPatrollingAbilityClass;

	// Minimum and maximum wait time between patrol points
	UPROPERTY(EditAnywhere, Category = "Patrolling")
	float MinPatrolWaitTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Patrolling")
	float MaxPatrolWaitTime = 3.0f;

	// Returns a random wait time in range
	UFUNCTION(BlueprintCallable, Category = "Patrolling")
	float GetRandomPatrolWaitTime() const;

private:
	UPROPERTY()
	class UGAS_AbilitySystemComponent* OwnerEnemyASC;

	UPROPERTY()
	class AGAS_EnemyBase* OwnerEnemy;

	UPROPERTY()
	UGAS_GameplayAbilityBase* LastMoveToLocationAbility = nullptr;

	int32 CurrentIndex = 0;
	bool bInPatrolling = false;
};

// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyPatrolling.h"
#include "AC_PatrolHandler.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPatrollingStopped);

class UMovementSingleData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_PatrolHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_PatrolHandler();

	UFUNCTION(BlueprintCallable)
	void StartPatrolling(UMovementSingleData* MovementData);

	UFUNCTION(BlueprintCallable)
	void StopPatrolling();

	FOnPatrollingStopped OnPatrollingStopped;

protected:
	virtual void BeginPlay() override;

	void MoveToNextPatrolPoint();

	void ActivatePatrollingAbility();

	UFUNCTION()
	void OnPatrollingAbilityEnded(const FCustomAbilityEndedData& ShadowAttackAbilityEndedData);

	// Returns a random wait time in range
	UFUNCTION(BlueprintCallable, Category = "Patrolling")
	float GetRandomPatrolWaitTime() const;

	UPROPERTY(EditAnywhere, Category = "Patrolling")
	float MinPatrolWaitTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Patrolling")
	float MaxPatrolWaitTime = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Patrolling")
	TArray<AActor*> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "Patrolling")
	TSubclassOf<UGA_EnemyPatrolling> EnemyPatrollingAbilityClass;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	class UGAS_AbilitySystemComponent* OwnerEnemyASC;

	UPROPERTY()
	class AGAS_EnemyBase* OwnerEnemy;

	UPROPERTY()
	UGAS_GameplayAbilityBase* LastPatrolingAbility = nullptr;

	FTimerHandle WaitForNextPatrolTimerHandle;
	int32 CurrentIndex = 0;
	bool bInPatrolling = false;

	UPROPERTY()
	UMovementSingleData* ChacedMovementData;
};

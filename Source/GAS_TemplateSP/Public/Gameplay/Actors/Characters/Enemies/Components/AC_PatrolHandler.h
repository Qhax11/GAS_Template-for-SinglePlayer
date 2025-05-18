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

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray<AActor*> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TSubclassOf<UGA_EnemyPatrolling> EnemyPatrollingAbilityClass;

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

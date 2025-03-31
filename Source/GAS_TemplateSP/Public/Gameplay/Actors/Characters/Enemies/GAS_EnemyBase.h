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
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USceneComponent* GetFinisherPointComponent() const { return FinisherPointComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UAC_EnemyMeleeComboManager* GetEnemyMeleeComboManagerComponent() const { return EnemyMeleeComboManagerComponent; }

protected:

	//* Components *//
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Components")
	class UAC_EnemyRespawn* EnemyRespawnComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Components")
	class UAC_EnemyMeleeComboManager* EnemyMeleeComboManagerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Components")
    USceneComponent* FinisherPointComponent;
};

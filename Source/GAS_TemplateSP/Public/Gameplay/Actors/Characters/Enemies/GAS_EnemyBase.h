// Qhax's GAS Template for 2D SinglePlayer

#pragma once

#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "GAS_EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API AGAS_EnemyBase : public AGAS_CharacterBase
{
	GENERATED_BODY()

public:
	AGAS_EnemyBase(const class FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;

protected:

	//* Components *//
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Components")
	class UAC_EnemyRespawn* EnemyRespawnComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Components")
	class UAC_MeleeComboManager* MeleeComboManagerComponent;

};

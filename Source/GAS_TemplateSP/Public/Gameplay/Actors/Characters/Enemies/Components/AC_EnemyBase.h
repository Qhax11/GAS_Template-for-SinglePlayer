// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_EnemyBase.generated.h"

class AGAS_EnemyBase;
class AAIControllerBase;
class UGAS_AbilitySystemComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_EnemyBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_EnemyBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	AAIControllerBase* OwnerController;

	UPROPERTY()
	AGAS_EnemyBase* OwnerEnemy;

	UPROPERTY()
	UGAS_AbilitySystemComponent* OwnerEnemyASC;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};

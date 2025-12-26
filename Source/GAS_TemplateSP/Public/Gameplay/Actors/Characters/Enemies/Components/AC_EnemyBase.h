// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "AC_EnemyBase.generated.h"

class AGAS_EnemyBase;
class AAIControllerBase;
class UGAS_AbilitySystemComponent;
class UAC_HeroMovementListener;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_EnemyBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_EnemyBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHeroSpawned(const FHeroSpawnData& HeroSpawnData);

	UPROPERTY()
	AAIControllerBase* OwnerController;

	UPROPERTY()
	AGAS_EnemyBase* OwnerEnemy;

	UPROPERTY()
	UGAS_AbilitySystemComponent* OwnerEnemyASC;

	UPROPERTY()
	AGAS_HeroBase* Hero;

	UPROPERTY()
	UGAS_AbilitySystemComponent* HeroASC;

	UPROPERTY()
	UAC_HeroMovementListener* HeroMovementListener;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};

// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"

UAC_EnemyBase::UAC_EnemyBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_EnemyBase::BeginPlay()
{
	Super::BeginPlay();

	OwnerEnemy = Cast<AGAS_EnemyBase>(GetOwner());
	if (!OwnerEnemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy is null in: %s)"), *GetName());
		return;
	}

	OwnerEnemyASC = Cast<UGAS_AbilitySystemComponent>(OwnerEnemy->GetAbilitySystemComponent());
	if (!OwnerEnemyASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyASC is null in %s, cannot initialize HeroControl."), *GetName());
		return;
	}

	OwnerController = Cast<AAIControllerBase>(OwnerEnemy->GetController());
    if (!OwnerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyController is null in: %s"), *GetName());
        return;
    }
}

void UAC_EnemyBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


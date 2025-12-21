// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_AIControllerBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include <Kismet/GameplayStatics.h>

UAC_AIControllerBase::UAC_AIControllerBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_AIControllerBase::BeginPlay()
{
	Super::BeginPlay();

    OwnerController = Cast<AAIControllerBase>(GetOwner());
    if (!OwnerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerController is null in: %s !"), *GetName());
        return;
    }

    IntendManager = OwnerController->GetIntendManagerComponent();
    if (!IntendManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("MovementManager is null in: %s !"), *GetName());
        return;
    }

    OwnerEnemyBase = Cast<AGAS_EnemyBase>(OwnerController->GetPawn());
    if (!OwnerEnemyBase)
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerEnemyBase is null in: %s !"), *GetName());
        return;
    }

    MovementManager = OwnerEnemyBase->GetEnemyMovementManagerComponent();
    if (!MovementManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("MovementManager is null in: %s !"), *GetName());
        return;
    }

    OwnerEnemyASC = Cast<UGAS_AbilitySystemComponent>(OwnerEnemyBase->GetAbilitySystemComponent());
    if (!OwnerEnemyASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("OwnerEnemyASC is null in: %s !"), *GetName());
        return;
    }

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    HeroBase = Cast<AGAS_HeroBase>(PlayerCharacter);
    if (!HeroBase)
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s !"), *GetName());
        return;
    }
}




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
    checkf(OwnerController, TEXT("OwnerController is null in %s"), *GetClass()->GetName());

    OwnerEnemyBase = Cast<AGAS_EnemyBase>(OwnerController->GetPawn());
    checkf(OwnerEnemyBase, TEXT("OwnerEnemyBase is null in %s"), *GetClass()->GetName());

    OwnerEnemyASC = Cast<UGAS_AbilitySystemComponent>(OwnerEnemyBase->GetAbilitySystemComponent());
    checkf(OwnerEnemyASC, TEXT("OwnerEnemyASC is null in %s"), *GetClass()->GetName());

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    HeroBase = Cast<AGAS_HeroBase>(PlayerCharacter);
    checkf(HeroBase, TEXT("HeroBase is null in %s"), *GetClass()->GetName());
}




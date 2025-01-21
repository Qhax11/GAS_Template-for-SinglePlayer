// Qhax's GAS Template for 2D SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyRespawn.h"
#include "Gameplay/Components/AC_MeleeComboManager.h"


AGAS_EnemyBase::AGAS_EnemyBase(const class FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UAC_EnemyRespawn>(TEXT("RespawnBase")))
{
	MeleeComboManagerComponent = CreateDefaultSubobject<UAC_MeleeComboManager>(TEXT("MeleeComboManagerComponent"));
}

void AGAS_EnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

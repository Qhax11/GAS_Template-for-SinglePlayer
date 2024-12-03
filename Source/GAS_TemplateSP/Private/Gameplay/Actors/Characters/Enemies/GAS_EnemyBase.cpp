// Qhax's GAS Template for 2D SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyRespawn.h"


AGAS_EnemyBase::AGAS_EnemyBase(const class FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	EnemyRespawnComponent = CreateDefaultSubobject<UAC_EnemyRespawn>(TEXT("EnemyRespawnComponent"));
}

void AGAS_EnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

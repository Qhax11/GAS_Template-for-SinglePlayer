// Qhax's GAS Template for 2D SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyRespawn.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"


AGAS_EnemyBase::AGAS_EnemyBase(const class FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UAC_EnemyRespawn>(TEXT("RespawnBase")))
{
	EnemyMeleeComboManagerComponent = CreateDefaultSubobject<UAC_EnemyMeleeComboManager>(TEXT("EnemyMeleeComboManager"));

	EnemyMovementManagerComponent = CreateDefaultSubobject<UAC_EnemyMovementManager>(TEXT("EnemyMovementManagerComponent"));

	FinisherPointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("FinisherPointComponent"));
	FinisherPointComponent->SetupAttachment(RootComponent);
}

void AGAS_EnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
	{
		SpawnDelegatesSubsystem->OnEnemySpawn.Broadcast(FCharacterSpawnData(this, GetAbilitySystemComponent()));
	}
}

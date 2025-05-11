// Qhax's GAS Template for 2D SinglePlayer


#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyRespawn.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"


AGAS_EnemyBase::AGAS_EnemyBase(const class FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UAC_EnemyRespawn>(TEXT("RespawnBase")))
{
	EnemyMeleeComboManagerComponent = CreateDefaultSubobject<UAC_EnemyMeleeComboManager>(TEXT("EnemyMeleeComboManager"));

	EnemyMovementManagerComponent = CreateDefaultSubobject<UAC_EnemyMovementManager>(TEXT("EnemyMovementManagerComponent"));

	EnemyStateManagerComponent = CreateDefaultSubobject<UAC_StateManager>(TEXT("EnemyStateManagerComponent"));

	FinisherPointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("FinisherPointComponent"));
	FinisherPointComponent->SetupAttachment(RootComponent);
}

void AGAS_EnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGAS_EnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	EnemyController = Cast<AAIControllerBase>(NewController);
	if (!EnemyController) 
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyController is null in: %s"), *GetName());
		return;
	}

	if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
	{
		FEnemySpawnData EnemySpawnData = FEnemySpawnData(this, GetAbilitySystemComponent(), EnemyController->GetStateTreeComponent());
		SpawnDelegatesSubsystem->OnEnemySpawn.Broadcast(EnemySpawnData);
	}
}

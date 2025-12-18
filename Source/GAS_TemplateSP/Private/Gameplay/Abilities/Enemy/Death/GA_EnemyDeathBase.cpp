// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Death/GA_EnemyDeathBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "AIController.h"
#include "BrainComponent.h"

void UGA_EnemyDeathBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AGAS_EnemyBase* Enemy = Cast<AGAS_EnemyBase>(GetAvatarActorFromActorInfo());
	if (!Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyDeathBase: Enemy is null in: %s"), *GetName());
		return;
	}

	AAIControllerBase* EnemyController = Cast<AAIControllerBase>(Enemy->GetController());
	if (!EnemyController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyDeathBase: EnemyController is null in: %s"), *GetName());
		return;
	}

	UBrainComponent* EnemyBrainComponent = EnemyController->GetBrainComponent();
	if (!EnemyBrainComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyDeathBase: EnemyBrainComponent is null in: %s"), *GetName());
		return;
	}

	EnemyController->GetEnemyStateManagerComponent()->StopLogic();
	EnemyController->GetBrainComponent()->StopLogic(TEXT("Enemey is dead"));
}

void UGA_EnemyDeathBase::BroadcastDeSpawn(const FCharacterDeSpawnData& DespawnData)
{
	if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
	{
		SpawnDelegatesSubsystem->UnregisterEnemy(DespawnData);
	}
}



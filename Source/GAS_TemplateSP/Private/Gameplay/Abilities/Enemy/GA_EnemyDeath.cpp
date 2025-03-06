// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/GA_EnemyDeath.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "AIController.h"
#include "BrainComponent.h"


void UGA_EnemyDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APawn* OwnerPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!OwnerPawn) 
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerPawn is null in: %s"), *GetName());
		return;
	}

	AAIController* EnemyController = Cast<AAIController>(OwnerPawn->GetController());
	if (!EnemyController)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyController is null in: %s"), *GetName());
		return;
	}

	UBrainComponent* EnemyBrainComponent = EnemyController->GetBrainComponent();
	if (!EnemyBrainComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyBrainComponent is null in: %s"), *GetName());
		return;
	}

	EnemyController->GetBrainComponent()->StopLogic(TEXT("Enemey is dead"));
}

void UGA_EnemyDeath::BroadcastDeSpawn()
{
	if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
	{
		SpawnDelegatesSubsystem->OnEnemyDeSpawn.Broadcast(Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo()));
	}
}

// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyMovementBase.h"
#include "Gameplay/Abilities/Tasks/AT_AIMoveTo.h"

UGA_EnemyMovementBase::UGA_EnemyMovementBase()
{
	AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Movement);
}

void UGA_EnemyMovementBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		FGameplayTagContainer CancelTags;
		CancelTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Movement);
		ASC->CancelAbilities(&CancelTags, nullptr, this);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	EnemyCharacter = Cast<AGAS_EnemyBase>(GetAvatarActorFromActorInfo());
	if (!EnemyCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: EnemyCharacter is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	EnemyMovementComp = EnemyCharacter->GetCharacterMovement();
	if (!EnemyMovementComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: EnemyMovementComp is null in: %s!, Ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	EnemyController = Cast<AAIControllerBase>(EnemyCharacter->GetController());
	if (!EnemyController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyMovementBase: EnemyController is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	EnemyMovementComp->MaxWalkSpeed = MovementSpeed;
}

void UGA_EnemyMovementBase::EndAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (bWasCancelled)
	{
		if (EnemyController)
		{
			EnemyController->StopMovement();
		}
	}

	// Super::EndAbility must be called last because it broadcasts the end event immediately.
	// Calling it early may trigger delegates or cleanup logic before this ability finishes its own cleanup.
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


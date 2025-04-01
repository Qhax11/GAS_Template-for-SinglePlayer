// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyStrafingBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_EnemyStrafingBase::UGA_EnemyStrafingBase()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Moving_Strafing);
}

void UGA_EnemyStrafingBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UCharacterMovementComponent* EnemyMovementComp = EnemyCharacter->GetCharacterMovement();
	if (!EnemyMovementComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyMovementComp is null in: %s!, Ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	EnemyCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = StrafingSpeed;
	EnemyCharacter->bUseControllerRotationYaw = false;

	StartEQSForStrafingLocation();
}

void UGA_EnemyStrafingBase::StartEQSForStrafingLocation()
{
	if (!EQSQueryTemplate)
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS Query Template is not set in: %s!"), *GetName());
		return;
	}

	FEnvQueryRequest QueryRequest(EQSQueryTemplate, EnemyController);

	QueryRequest.Execute(QueryRunMode, this, &UGA_EnemyStrafingBase::OnStrafingLocationQueryFinished);
}

void UGA_EnemyStrafingBase::OnStrafingLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (!Result.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS Query did not return any results in: %s"), *GetName());
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	FVector BestLocation = Result->GetItemAsLocation(0);

	EnemyController->MoveToLocation(BestLocation, AcceptanceRadius, false, true, true, true, nullptr, false);
}

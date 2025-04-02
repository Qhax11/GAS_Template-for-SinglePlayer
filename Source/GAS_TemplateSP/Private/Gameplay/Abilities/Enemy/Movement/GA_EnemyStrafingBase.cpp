// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyStrafingBase.h"

UGA_EnemyStrafingBase::UGA_EnemyStrafingBase()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Moving_Strafing);

	TEnumAsByte<EGameplayAbilityTriggerSource::Type> TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	FAbilityTriggerData TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = TriggerSource;
	TriggerData.TriggerTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_AI_Movement_Strafing;

	AbilityTriggers.Add(TriggerData);
}

void UGA_EnemyStrafingBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	EnemyMovementComp->bOrientRotationToMovement = false;
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

	RequestMoveToLocation(BestLocation);
}

void UGA_EnemyStrafingBase::EndAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, 
	bool bWasCancelled)
{
	EnemyMovementComp->bOrientRotationToMovement = true;
	EnemyCharacter->bUseControllerRotationYaw = true;

	// Super::EndAbility must be called last because it broadcasts the end event immediately.
    // Calling it early may trigger delegates or cleanup logic before this ability finishes its own cleanup.
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

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

	if (!TriggerEventData)
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerEventData is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	if (TriggerEventData->InstigatorTags.IsValidIndex(0))
	{
		FGameplayTag DirectionTag = TriggerEventData->InstigatorTags.GetByIndex(0);
		StartEQSForStrafingLocation(DirectionTag);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no direction tag in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
}

void UGA_EnemyStrafingBase::StartEQSForStrafingLocation(FGameplayTag StrafeDirectionTag)
{
	if (!EQSQueryTemplate)
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS Query Template is not set in: %s!"), *GetName());
		return;
	}

	FEnvQueryRequest QueryRequest(EQSQueryTemplate, EnemyController);

	float DirectionFloat = ConvertStrafeDirectionTagToFloat(StrafeDirectionTag);

	QueryRequest.SetFloatParam(FName("StrafeDirectionParam"), DirectionFloat);

	QueryRequest.Execute(QueryRunMode, this, &UGA_EnemyStrafingBase::OnStrafingLocationQueryFinished);
}

float UGA_EnemyStrafingBase::ConvertStrafeDirectionTagToFloat(FGameplayTag StrafeDirectionTag)
{
	float TagValue = 2.0f; // Default: Both

	if (StrafeDirectionTag.MatchesTagExact(GAS_Tags::TAG_Gameplay_Utilities_Direction_Left))
	{
		TagValue = 0.0f;
	}
	else if (StrafeDirectionTag.MatchesTagExact(GAS_Tags::TAG_Gameplay_Utilities_Direction_Right))
	{
		TagValue = 1.0f;
	}

	return TagValue;
}

void UGA_EnemyStrafingBase::OnStrafingLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (!Result.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS result invalid in: %s"), *GetName());
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
	// Super::EndAbility must be called last because it broadcasts the end event immediately.
    // Calling it early may trigger delegates or cleanup logic before this ability finishes its own cleanup.
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

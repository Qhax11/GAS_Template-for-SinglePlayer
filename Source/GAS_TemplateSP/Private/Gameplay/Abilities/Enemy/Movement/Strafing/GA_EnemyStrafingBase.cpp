// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/Strafing/GA_EnemyStrafingBase.h"
#include "Gameplay/Utilities/Combat/CombatDistanceUtils.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Gameplay/Abilities/Tasks/AT_AIMoveTo.h"

UGA_EnemyStrafingBase::UGA_EnemyStrafingBase()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Moving_Strafing);

	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Moving_Strafing);
}

void UGA_EnemyStrafingBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!TriggerEventData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyStrafingBase: TriggerEventData is null in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	if (TriggerEventData->InstigatorTags.IsValidIndex(0))
	{
		FGameplayTag DirectionTag = TriggerEventData->InstigatorTags.GetByIndex(0);
		ExecuteFindLocationQuery(DirectionTag);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyStrafingBase: There is no direction tag in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	CachedExpectedDuration = TriggerEventData->EventMagnitude;
}

void UGA_EnemyStrafingBase::ExecuteFindLocationQuery(FGameplayTag StrafeDirectionTag)
{
	if (!EQSQueryTemplate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyStrafingBase: EQS Query Template is not set in: %s!"), *GetName());
		return;
	}

	FEnvQueryRequest QueryRequest(EQSQueryTemplate, EnemyController);
	float DirectionFloat = ConvertDirectionTagToFloat(StrafeDirectionTag);
	QueryRequest.SetFloatParam(FName("StrafeDirectionParam"), DirectionFloat);
	QueryRequest.Execute(QueryRunMode, this, &UGA_EnemyStrafingBase::OnLocationQueryFinished);
}

float UGA_EnemyStrafingBase::ConvertDirectionTagToFloat(FGameplayTag StrafeDirectionTag)
{
	float TagValue = 2.0f; // Default: Both

	if (StrafeDirectionTag.MatchesTagExact(GAS_Tags::TAG_Gameplay_Direction_Left))
	{
		TagValue = 0.0f;
	}
	else if (StrafeDirectionTag.MatchesTagExact(GAS_Tags::TAG_Gameplay_Direction_Right))
	{
		TagValue = 1.0f;
	}

	return TagValue;
}

void UGA_EnemyStrafingBase::OnLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (!Result.IsValid() || Result->Items.Num() == 0)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	const FVector BestLocation = Result->GetItemAsLocation(0);

	// TEK satır! Her şey task içinde halloluyor
	UAT_AIMoveTo* MoveTask = UAT_AIMoveTo::AIMoveTo(
		this,
		FName("StrafeMove"),
		EnemyController,
		BestLocation,
		AcceptanceRadius,
		DEFAULT_MIN_MOVEMENT_DURATION,  // Min duration support built-in!
		MovementSpeed
	);

	ExecuteMoveTask(MoveTask);
}

void UGA_EnemyStrafingBase::OnMoveCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_EnemyStrafingBase::OnMoveAborted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UGA_EnemyStrafingBase::OnMoveFailed()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}



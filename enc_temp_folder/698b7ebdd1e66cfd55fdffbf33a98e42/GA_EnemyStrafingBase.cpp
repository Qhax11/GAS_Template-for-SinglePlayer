// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/Strafing/GA_EnemyStrafingBase.h"
#include "Gameplay/Utilities/Combat/CombatDistanceUtils.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

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

	UE_LOG(LogTemp, Warning, TEXT(">>> Strafe Ability ACTIVATED: %s"), *GetName());

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
	UE_LOG(LogTemp, Warning, TEXT(">>> CachedExpectedDuration: %.2f"), CachedExpectedDuration);
}

void UGA_EnemyStrafingBase::ExecuteFindLocationQuery(FGameplayTag StrafeDirectionTag)
{
	if (!EQSQueryTemplate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyStrafingBase: EQS Query Template is not set in: %s!"), *GetName());
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

void UGA_EnemyStrafingBase::OnStrafingLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	UE_LOG(LogTemp, Warning, TEXT(">>> EQS Query FINISHED"));

	if (!Result.IsValid() || Result->Items.Num() == 0)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	const FVector BestLocation = Result->GetItemAsLocation(0);
	const float Distance = CombatDistance::GetDistance2D(EnemyCharacter, BestLocation);

	UE_LOG(LogTemp, Warning, TEXT(">>> EQS Best Location: %s, Distance: %.2f"), *BestLocation.ToString(), Distance);

	if (Distance < MinStrafeDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyStrafingBase: Strafe loc too close (%.1f)"), Distance);
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT(">>> Requesting MOVE TO LOCATION"));
	RequestMoveToLocation(BestLocation);

	UE_LOG(LogTemp, Warning, TEXT(">>> Activating WAIT DELAY TASK"));
	ActivateWaitDelayTask();
}

void UGA_EnemyStrafingBase::ActivateWaitDelayTask()
{
	float ExpectedDuration = CachedExpectedDuration;
	ExpectedDuration = FMath::Max(ExpectedDuration, DEFAULT_MIN_MOVEMENT_DURATION);

	UE_LOG(LogTemp, Warning, TEXT(">>> Wait Task Duration: %.2f"), ExpectedDuration);

	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, ExpectedDuration);
	WaitTask->OnFinish.AddDynamic(this, &UGA_EnemyStrafingBase::OnStrafingTimeEnd);
	WaitTask->ReadyForActivation();
}

void UGA_EnemyStrafingBase::OnStrafingTimeEnd()
{
	UE_LOG(LogTemp, Warning, TEXT(">>> STRAFE TIME ENDED"));

	if (EnemyController)
	{
		EnemyController->StopMovement();
	}

	// It wasn't cancelled, we want this.
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}


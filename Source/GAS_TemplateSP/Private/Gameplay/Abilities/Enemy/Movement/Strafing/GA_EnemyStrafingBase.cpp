// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Movement/Strafing/GA_EnemyStrafingBase.h"
#include "Gameplay/Utilities/Combat/CombatDistanceUtils.h"

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

	if (!TriggerEventData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyStrafingBase: TriggerEventData is null in: %s, ability cannot initialize"), *GetName());
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
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyStrafingBase: There is no direction tag in: %s, ability cannot initialize"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	float ExpectedDuration = TriggerEventData->EventMagnitude;
	ExpectedDuration = FMath::Max(ExpectedDuration, 0.25f);
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			MovementTimerHandle,
			this,
			&UGA_EnemyStrafingBase::OnStrafingTimeEnd,
			ExpectedDuration,
			false
		);
	}
}

void UGA_EnemyStrafingBase::StartEQSForStrafingLocation(FGameplayTag StrafeDirectionTag)
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
	if (!Result.IsValid() || Result->Items.Num() == 0)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	const FVector BestLocation = Result->GetItemAsLocation(0);
	const float Distance = CombatDistance::GetDistance2D(EnemyCharacter, BestLocation);
	if (Distance < MinStrafeDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGA_EnemyStrafingBase: Strafe loc too close (%.1f)"), Distance);
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
		return;
	}

	RequestMoveToLocation(BestLocation);
}

void UGA_EnemyStrafingBase::OnStrafingTimeEnd()
{
	if (EnemyController)
	{
		EnemyController->StopMovement();
	}

	// It wasn't cancelled, we want this.
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}


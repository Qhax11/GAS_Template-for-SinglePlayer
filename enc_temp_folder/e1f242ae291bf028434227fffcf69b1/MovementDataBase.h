// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/DecisionOptionData.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "MovementDataBase.generated.h"

class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EMovementChainDisableReason : uint8
{
	None,
	OnCooldown,
	InvalidContext,
	BehaviorStateBlocked,
	CustomConditionFailed,
};

USTRUCT(BlueprintType)
struct FMovementEnableDebug
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EMovementChainDisableReason DisableReason = EMovementChainDisableReason::None;

	UPROPERTY(BlueprintReadOnly)
	FString AdditionalInfo;
};

UENUM()
enum class EMovementChanceFailReason : uint8
{
	None,
	OutOfRange,
	RandomRollFailed,
};

USTRUCT(BlueprintType)
struct FMovementChanceDebug
{
	GENERATED_BODY()

	EMovementChanceFailReason ChanceFailReason = EMovementChanceFailReason::None;
	float Roll = 0.f;
	float Threshold = 0.f;
};

USTRUCT(BlueprintType)
struct FMovementScoreDebug
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float BiasScore = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float DistanceScore = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float IntentScore = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float TotalScore = 0.f;
};

USTRUCT(BlueprintType)
struct FMovementDecisionContext
{
	GENERATED_BODY()

	// ---- CORE ----
	UPROPERTY(BlueprintReadOnly)
	AActor* Owner = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AActor* Target = nullptr;

	// ---- SPATIAL ----
	UPROPERTY(BlueprintReadOnly)
	bool bIsTargetMoving = false;

	// ---- STATE ----
	UPROPERTY(BlueprintReadOnly)
	EEnemyIntent Intent = EEnemyIntent::LowPressure;

	// ---- FLOW CONTROL ----
	UPROPERTY(BlueprintReadOnly)
	float TimeSinceLastMovement = 0.f;

	// ---- AVAILABILITY ----
	UPROPERTY(BlueprintReadOnly)
	bool bIsAnyMovementAbilityOnCooldown = false;
};

UCLASS(Abstract, BlueprintType, EditInlineNew, DefaultToInstanced)
class GAS_TEMPLATESP_API UMovementDataBase : public UDecisionOptionData
{
	GENERATED_BODY()

public:
	virtual bool IsEnable(const FMovementDecisionContext& Context, FMovementEnableDebug* OutDebug = nullptr) const;

	virtual bool PassesChance(const FMovementDecisionContext& Context, FMovementChanceDebug* OutDebug = nullptr) const;

	virtual float GetScore(const FMovementDecisionContext& Context, FMovementScoreDebug* OutDebug = nullptr) const;
	
	virtual bool IsChain() const { return false; }

};

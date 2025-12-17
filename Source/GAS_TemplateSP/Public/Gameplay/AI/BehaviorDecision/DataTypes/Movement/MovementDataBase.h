// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/DecisionOptionData.h"
#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "MovementDataBase.generated.h"

class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EMovementDisableReason : uint8
{
	None                UMETA(DisplayName = "None"),
	OnCooldown          UMETA(DisplayName = "On Cooldown"),
	OutOfRange          UMETA(DisplayName = "Out Of Range"),
	InvalidContext      UMETA(DisplayName = "Invalid Context"),
	BehaviorStateBlocked UMETA(DisplayName = "Behavior State Blocked"),
	CustomConditionFailed UMETA(DisplayName = "Custom Condition Failed"),
};

USTRUCT(BlueprintType)
struct FMovementEnableDebug
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsEnabled = true;

	UPROPERTY(BlueprintReadOnly)
	EMovementDisableReason DisableReason = EMovementDisableReason::None;

	UPROPERTY(BlueprintReadOnly)
	FString AdditionalInfo;
};

UENUM()
enum class EMovementChanceFailReason : uint8
{
	None,
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
	float BaseScore = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float DistanceScore = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float BehaviorStateScore = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float TotalScore = 0.f;
};

USTRUCT(BlueprintType)
struct FMovementDecisionContext
{
	GENERATED_BODY()

	/* ================= CORE ACTORS ================= */

	UPROPERTY(BlueprintReadOnly)
	AActor* Owner = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AActor* Target = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* OwnerASC = nullptr;

	/* ================= SPATIAL ================= */

	UPROPERTY(BlueprintReadOnly)
	float DistanceToTarget = 0.f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsTargetMoving = false;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag TargetMoveDirection; // Forward / Backward / Left / Right

	/* ================= OWNER STATE ================= */

	UPROPERTY(BlueprintReadOnly)
	EBehaviorState BehaviorState = EBehaviorState::None;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer OwnerStateTags;

	/* ================= COMBAT ================= */

	UPROPERTY(BlueprintReadOnly)
	bool bHasIncomingAttack = false;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag IncomingAttackType; // Thrust / Sweep / Heavy

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag IncomingAttackDirection;

	/* ================= COOLDOWN / AVAILABILITY ================= */

	UPROPERTY(BlueprintReadOnly)
	bool bIsAnyMovementAbilityOnCooldown = false;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer ActiveMovementCooldownTags;

	/* ================= MISC ================= */

	UPROPERTY(BlueprintReadOnly)
	float TimeSinceLastMovement = 0.f;
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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Name of this Movement. Used for debugging or referencing in logic."))
	FName MovementName;

};

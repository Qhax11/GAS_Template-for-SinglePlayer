// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CombatTypes.generated.h"

USTRUCT(BlueprintType)
struct FComingAttackPayload
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UGAS_GameplayAbilityBase* ComingAttack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ComingAttackHitTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTagContainer ComingAttackTags;

	FComingAttackPayload()
		: ComingAttack(nullptr)
	{}

	FComingAttackPayload(UGAS_GameplayAbilityBase* InComingAttack, float InComingAttackHitTime, FGameplayTagContainer InComingAttackTags)
		: ComingAttack(InComingAttack), ComingAttackHitTime(InComingAttackHitTime), ComingAttackTags(InComingAttackTags)
	{}
};
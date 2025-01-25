// Qhax's GAS Template for SinglePlayer

#pragma once

#include "BehaviorTree/BTService.h"
#include "BTService_AddRemoveGameplayTag.generated.h"

class AGAS_CharacterBase;

UCLASS()
class GAS_TEMPLATESP_API UBTService_AddRemoveGameplayTag : public UBTService
{
	GENERATED_BODY()

public:
    UBTService_AddRemoveGameplayTag();

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector CurrentNodeKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FName DefinedNodeName;

    UPROPERTY(EditAnywhere, Category = "GameplayTag")
    FGameplayTagContainer TagsToAdd;

    UPROPERTY(EditAnywhere, Category = "GameplayTag")
    FGameplayTagContainer TagsToRemoved;

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector ActorKey;

    AGAS_CharacterBase* OwnerCharacter;
};

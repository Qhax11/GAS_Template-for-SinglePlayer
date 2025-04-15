// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "AIControllerBoss.generated.h"

USTRUCT(BlueprintType)
struct FMyStateTreePayload
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 SomeValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector TargetLocation;
};

UCLASS()
class GAS_TEMPLATESP_API AAIControllerBoss : public AAIControllerBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay();

	virtual void TargetPreceptionUpdated(AActor* Actor, FAIStimulus Stimulus) override;

	virtual bool RegisterTags(AGAS_CharacterBase* TargetCharacter) override;

	// Tag Listen Functions

	UFUNCTION()
	void OnDeadWithFinisherTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);


};

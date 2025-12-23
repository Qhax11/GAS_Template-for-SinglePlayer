// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AN_ExecuteGameplayCue.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAN_ExecuteGameplayCue : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (Categories = "GameplayCue"))
	FGameplayTag GameplayCueTag;
};

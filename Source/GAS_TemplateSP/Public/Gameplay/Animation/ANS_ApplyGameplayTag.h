// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "ANS_ApplyGameplayTag.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UANS_ApplyGameplayTag : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// .h dosyanýzda public alana eklenmeli:
	virtual FString GetNotifyName_Implementation() const;

	// EditAnywhere allows you to pick the tag in the Animation Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Gameplay.State.Phase"))
	FGameplayTag TagToApply;
};

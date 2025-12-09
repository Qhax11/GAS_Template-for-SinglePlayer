// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "ANS_AttackTrace.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UANS_AttackTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UANS_AttackTrace();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (Categories = "Gameplay.Event.AnimNotifyState"))
	FGameplayTag EventTagStart;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (Categories = "Gameplay.Event.AnimNotifyState"))
	FGameplayTag EventTagContinue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (Categories = "Gameplay.Event.AnimNotifyState"))
	FGameplayTag EventTagEnd;
};

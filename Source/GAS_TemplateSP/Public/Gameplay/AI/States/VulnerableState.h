// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "VulnerableState.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UVulnerableState : public UStateBase
{
	GENERATED_BODY()

public:
	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;
	
	void ActivateVulnerableAbility();

	UFUNCTION()
	void OnVulnerableAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData);

	UFUNCTION()
	void OnHeroShadowFinisherAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData);

	virtual void OnExit_Implementation() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> VulnerableAbilityClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> HeroShadowFinisherAbilityClass;

	UPROPERTY()
	UGAS_GameplayAbilityBase* LastUsedActivatedVulnerableAbility;
};

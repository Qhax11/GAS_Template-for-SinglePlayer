// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroBase.h"
#include "AC_HeroAbilityBuffer.generated.h"

struct GameplayTagContainer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_HeroAbilityBuffer : public UAC_HeroBase
{
	GENERATED_BODY()

public:	
	UAC_HeroAbilityBuffer();

protected:
	virtual void BeginPlay() override;
		
	UFUNCTION()
	void OnAbilityFailed(const UGameplayAbility* FailedAbility, const FGameplayTagContainer& TagExplaining);

	UFUNCTION()
	void OnHeroPhaseActivePostHitTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UPROPERTY()
	TSubclassOf<UGameplayAbility> BufferedAbilityClass;

	UPROPERTY(EditDefaultsOnly)
	float BufferLifetime = 0.25f;

	FTimerHandle BufferTimerHandle;

	UPROPERTY()
	class UAC_HeroMeleeComboManager* HeroMeleeComboManager;

	void TryActivateBufferedAbility();

	void ClearBuffer();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

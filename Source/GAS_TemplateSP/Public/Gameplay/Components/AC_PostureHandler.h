// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_PostureHandler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_PostureHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_PostureHandler();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnAbilitySetGiven(const AActor* OwnerActor);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	virtual void OnHealthChanged(const FAttributeChangeCallbackData& Data);

	UFUNCTION()
	virtual void OnPostureChanged(const FAttributeChangeCallbackData& Data);

	UFUNCTION()
	void OnKnocbackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnDashTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	void TriggerPostureRegenEffect();

	class AGAS_CharacterBase* OwnerCharacter;
	class UAbilitySystemComponent* OwnerASC;
	FTimerHandle PostureRegenTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "PostureHandler|Regen")
	TSubclassOf<class UGameplayEffect> PostureRegenGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category = "PostureHandler|Regen")
	float PostureRegenPerTick = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "PostureHandler")
	float PostureRegenDelay = 3.f;
};

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

	UFUNCTION()
	virtual void OnHealthChanged(const FAttributeChangeCallbackData& Data);

	UFUNCTION()
	virtual void OnPostureChanged(const FAttributeChangeCallbackData& Data);

	UFUNCTION()
	void OnKnocbackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnDashTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	class AGAS_CharacterBase* OwnerCharacter;
	class UAbilitySystemComponent* OwnerASC;
};

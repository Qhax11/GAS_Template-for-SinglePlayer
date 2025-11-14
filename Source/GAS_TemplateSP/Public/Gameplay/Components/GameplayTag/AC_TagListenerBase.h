// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "AC_TagListenerBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_TagListenerBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_TagListenerBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class AGAS_CharacterBase* OwnerCharacter;

	UPROPERTY()
	class UAbilitySystemComponent* OwnerCharacterASC;

	UPROPERTY()
	class UCharacterMovementComponent* OwnerCharacterMoveComp;

	// Moving Tags
	UFUNCTION()
	void OnPatrollingTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnPatrollingTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnStrafingTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnStrafingTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnRunningTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnRunningTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);
};

// Qhax's GAS Template for SinglePlayer

#pragma once

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Templates/SubclassOf.h"
#include "Engine/DataAsset.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GAS_AbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityGiven, UAbilitySystemComponent*, ASC, FGameplayAbilitySpec&, AbilitySpec);

USTRUCT(BlueprintType)
struct FAbilityData
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "AbilityData")
	TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY(EditAnywhere, Category = "AbilityData")
	class UInputAction* AbilityInput;

};

USTRUCT(BlueprintType)
struct FAttributeSetData
{
	GENERATED_BODY()
public:

	/** Attribute Set to grant */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attributes)
	TSoftClassPtr<UAttributeSet> AttributeSet;

	/** Data table referent to initialize the attributes with, if any (can be left unset) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attributes, meta = (RequiredAssetDataTags = "RowStructure=/Script/GameplayAbilities.AttributeMetaData"))
	TSoftObjectPtr<UDataTable> InitializationData;
};

UCLASS()
class GAS_TEMPLATESP_API UGAS_GameplayAbilitySet : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySet")
	TArray<FAbilityData> Abilities;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySet")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySet")
	FAttributeSetData AttributeData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilitySet")
	FGameplayTagContainer PermenantTags;
};

// Delegate to broadcast when an ability with a specific tag ends
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayAbilityEnded_BP, FGameplayTag, AbilityTag);

UCLASS()
class GAS_TEMPLATESP_API UGAS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "GAS_AbilitySystemComponent")
	UGAS_GameplayAbilityBase* TryActivateAbilityByClassAndReturnInstance(TSubclassOf<UGameplayAbility> AbilityClass);

	bool GiveAbilitySet(const UGAS_GameplayAbilitySet* AbilitySet);
	
	void GiveAbilities(const UGAS_GameplayAbilitySet* AbilitySet);

	UFUNCTION(BlueprintCallable, Category = "GAS_AbilitySystemComponent")
	void GiveAbilityWithInputAction(UInputAction* AbilityInput, const TSubclassOf<UGameplayAbility> Ability);

	bool IsAbilityGivenAlready(const TSubclassOf<UGameplayAbility> Ability);

	void TryAbilityInputBind(UInputAction* AbilityInput, const FGameplayAbilitySpecHandle& AbilitySpecHandle);

	void GiveAttributes(const UGAS_GameplayAbilitySet* AbilitySet);

	void GivePermenantTags(FGameplayTagContainer PermenantTags);

	// Overrided for OnAbilityGiven broadcast
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityGiven OnAbilityGranted;
};

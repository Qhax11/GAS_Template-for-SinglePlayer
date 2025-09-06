// Qhax's GAS Template for SinglePlayer

#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_AbilityInputBinding.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"


void UGAS_AbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGAS_AbilitySystemComponent::TryActivateAbilityByEventData(const FGameplayEventData& EventData)
{
	if (!EventData.EventTag.IsValid()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("TryActivateAbilityByTagWithEventData: EventTag is invalid!"));
		return;
	}
	HandleGameplayEvent(EventData.EventTag, &EventData);
}

UGAS_GameplayAbilityBase* UGAS_AbilitySystemComponent::TryActivateAbilityByClassWithEventData(TSubclassOf<UGameplayAbility> AbilityClass, const FGameplayEventData& EventData)
{
	if (!AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryActivateAbilityByClassAndReturnInstance: AbilityClass is invalid!"));
		return nullptr;
	}

	UGAS_GameplayAbilityBase* const InAbilityCDO = AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability == InAbilityCDO)
		{
			TryActivateAbilityByEventData(EventData);
			UGameplayAbility* Instance = Spec.GetPrimaryInstance();
			if (Instance) 
			{
				return CastChecked<UGAS_GameplayAbilityBase>(Instance);
			}
			else
			{
				if (Spec.GetAbilityInstances().IsValidIndex(0)) 
				{
					return CastChecked<UGAS_GameplayAbilityBase>(Spec.GetAbilityInstances()[0]);
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("TryActivateAbilityByClassAndReturnInstance: No ability found for class %s!"), *AbilityClass->GetName());
	return nullptr;
}

UGAS_GameplayAbilityBase* UGAS_AbilitySystemComponent::TryActivateAbilityByClassAndReturnInstance(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (!AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryActivateAbilityByClassAndReturnInstance: AbilityClass is invalid!"));
		return nullptr;
	}

	UGAS_GameplayAbilityBase* const InAbilityCDO = AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability == InAbilityCDO)
		{
			if (TryActivateAbility(Spec.Handle))
			{
				if (UGAS_GameplayAbilityBase* PrimaryInstance = Cast<UGAS_GameplayAbilityBase>(Spec.GetPrimaryInstance()))
				{
					return PrimaryInstance;
				}
				else
				{
					return InAbilityCDO;
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("TryActivateAbilityByClassAndReturnInstance: No ability found for class %s!"), *AbilityClass->GetName());
	return nullptr;
}

bool UGAS_AbilitySystemComponent::GiveAbilitySet(const UGAS_GameplayAbilitySet* AbilitySet)
{
	if (AbilitySet)
	{
		GiveAbilities(AbilitySet);
		GiveAttributes(AbilitySet);
		GivePermenantTags(AbilitySet->PermenantTags);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySet doesen't exist in %s"), *GetOwnerActor()->GetName());
		return false;
	}
}

void UGAS_AbilitySystemComponent::GiveAbilities(const UGAS_GameplayAbilitySet* AbilitySet)
{
	if (AbilitySet->Abilities.IsEmpty())
	{
		return;
	}

	for (const FAbilityData& AbilityData : AbilitySet->Abilities)
	{
		GiveAbilityWithAbilityData(AbilityData);
	}
}

void UGAS_AbilitySystemComponent::GiveAbilityWithAbilityData(const FAbilityData& AbilityData)
{
	if(AbilityData.Ability && !IsAbilityGivenAlready(AbilityData.Ability))
	{
		FGameplayAbilitySpecHandle GivenAbilitySpecHandle = GiveAbility(FGameplayAbilitySpec(AbilityData.Ability.Get()));
		TryAbilityInputBind(AbilityData.AbilityInput.Get(), GivenAbilitySpecHandle);
	}
}

bool UGAS_AbilitySystemComponent::IsAbilityGivenAlready(const TSubclassOf<UGameplayAbility> Ability)
{
	TArray<FGameplayAbilitySpec>& LocalActivatableAbilities = GetActivatableAbilities();
	for (FGameplayAbilitySpec Spec : LocalActivatableAbilities)
	{
		if (Spec.Ability == Ability.GetDefaultObject())
		{
			return true;
		}
	}
	return false;
}

void UGAS_AbilitySystemComponent::TryAbilityInputBind(UInputAction* AbilityInput, const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{
	if (UAC_AbilityInputBinding* AbilityInputBinding = Cast<UAC_AbilityInputBinding>(GetAvatarActor()->FindComponentByClass(UAC_AbilityInputBinding::StaticClass())))
	{
		AbilityInputBinding->SetInputBinding(AbilityInput, AbilitySpecHandle);
	}
}

void UGAS_AbilitySystemComponent::GiveAttributes(const UGAS_GameplayAbilitySet* AbilitySet)
{
	if (!IsValid(GetOwnerActor()))
	{
		return;
	}

	const TSubclassOf<UAttributeSet> ToBeAssignedAttributeSet = AbilitySet->AttributeData.AttributeSet.LoadSynchronous();
	if (!ToBeAssignedAttributeSet)
	{
		return;
	}

	UAttributeSet* OutAttributeSet = NewObject<UAttributeSet>(GetOwnerActor(), ToBeAssignedAttributeSet);

	if (!AbilitySet->AttributeData.InitializationData.IsNull())
	{
		const UDataTable* InitData = AbilitySet->AttributeData.InitializationData.LoadSynchronous();
		if (InitData)
		{
			OutAttributeSet->InitFromMetaDataTable(InitData);
		}
	}

	AddAttributeSetSubobject(OutAttributeSet);
}

void UGAS_AbilitySystemComponent::GivePermenantTags(FGameplayTagContainer PermenantTags)
{
	if (!PermenantTags.IsEmpty()) 
	{
		AddLooseGameplayTags(PermenantTags);
	}
}

void UGAS_AbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	OnAbilityGranted.Broadcast(this ,AbilitySpec);
}


// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"


UGameplayEffect* UGAS_EffectBlueprintFunctionLibary::CreateEffectWithTSubclass(TSubclassOf<UGameplayEffect> EffectSubClass)
{
	if (!EffectSubClass) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create EffectSubClass in GAS_EffectBlueprintFunctionLibary."));
		return nullptr;
	}

	UGameplayEffect* GameplayEffectClass = EffectSubClass->GetDefaultObject<UGameplayEffect>();
	if (!GameplayEffectClass) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create UGameplayEffect in GAS_EffectBlueprintFunctionLibary."));
		return nullptr;
	}

	return GameplayEffectClass;
}

bool UGAS_EffectBlueprintFunctionLibary::CreateInstantEffectSpecWithSetByCallerValueWithMoreData(FGameplayEffectSpec& OutSpec,
	UAbilitySystemComponent* SourceASC,
	TSubclassOf<UGameplayEffect> GameplayEffectClass,
	const FGameplayTag SetByCallerTag,
	float SetByCallerValue,
	const FHitResult& InHitResult,
	const UGameplayAbility* InGameplayAbility)
{
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	EffectContext.AddHitResult(InHitResult);
	EffectContext.SetAbility(InGameplayAbility);
	FGameplayEffectSpecHandle NewHandle = SourceASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FGameplayEffectSpec* EffectSpec = NewHandle.Data.Get();
		if (EffectSpec)
		{
			if (SetByCallerTag.IsValid() && SetByCallerValue != 0)
			{
				EffectSpec->SetSetByCallerMagnitude(SetByCallerTag, SetByCallerValue);
			}
			OutSpec = *EffectSpec;
			return true;
		}
	}

	return false;
}

bool UGAS_EffectBlueprintFunctionLibary::CreateEffectSpecWithSetByCallerValue(FGameplayEffectSpec& OutSpec,
	UAbilitySystemComponent* SourceASC, 
	TSubclassOf<UGameplayEffect> GameplayEffectClass, 
	const FGameplayTag SetByCallerTag, 
	float SetByCallerValue)
{
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	FGameplayEffectSpecHandle NewHandle = SourceASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FGameplayEffectSpec* EffectSpec = NewHandle.Data.Get();
		if (EffectSpec)
		{
			if (SetByCallerTag.IsValid() && SetByCallerValue != 0)
			{
				EffectSpec->SetSetByCallerMagnitude(SetByCallerTag, SetByCallerValue);
			}
			OutSpec = *EffectSpec;
			return true;
		}
	}

	return false;
}

UGameplayEffect* UGAS_EffectBlueprintFunctionLibary::CreateInstantEffectWithModifier(FGameplayAttribute ModifierAttribute, TEnumAsByte<EGameplayModOp::Type> ModifierOp, float ModifierValue)
{
	UGameplayEffect* GenericEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("GenericEffect")));
	if (!GenericEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create UGameplayEffect in GAS_EffectBlueprintFunctionLibary."));
		return nullptr;
	}

	GenericEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo NewModifier;
	NewModifier.ModifierMagnitude = FScalableFloat(ModifierValue);
	NewModifier.ModifierOp = ModifierOp;
	NewModifier.Attribute = ModifierAttribute;

	GenericEffect->Modifiers.Add(NewModifier);

	return GenericEffect;
}

UGameplayEffect* UGAS_EffectBlueprintFunctionLibary::CreateDurationEffectWithModifiers(FGameplayAttribute ModifierAttribute, TEnumAsByte<EGameplayModOp::Type> ModifierOp, float ModifierValue, float Duration)
{
	UGameplayEffect* DurationEffect = CreateInstantEffectWithModifier(ModifierAttribute, ModifierOp, ModifierValue);
	if (!DurationEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create UGameplayEffect in GAS_EffectBlueprintFunctionLibary."));
		return nullptr;
	}

	DurationEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationEffect->DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(Duration));
	DurationEffect->Period = 0;

	return DurationEffect;
}

UGameplayEffect* UGAS_EffectBlueprintFunctionLibary::CreateDurationEffectWithGrantedTags(TArray<FGameplayTag> TagsToAppend, float Duration)
{
	UGameplayEffect* GenericEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("GenericEffect")));
	if (!GenericEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create UGameplayEffect in GAS_EffectBlueprintFunctionLibary."));
		return nullptr;
	}

	GenericEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	GenericEffect->DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(Duration));
	GenericEffect->Period = 0;

	// TODO: Fix tags are not removed after the effect end, check!
	for (FGameplayTag GameplayTag : TagsToAppend)
	{
		GenericEffect->CachedGrantedTags.AddTag(GameplayTag);
	}

	return GenericEffect;
}

void UGAS_EffectBlueprintFunctionLibary::AddTagsToEffectSpecWithArray(FGameplayEffectSpec& Spec, TArray<FGameplayTag> TagsToAdd)
{
	FGameplayTagContainer Container = FGameplayTagContainer();
	for (FGameplayTag GameplayTag : TagsToAdd)
	{
		Container.AddTag(GameplayTag);
	}

	Spec.AppendDynamicAssetTags(Container);
}

void UGAS_EffectBlueprintFunctionLibary::AddTagsToEffectSpecWithContain(FGameplayEffectSpec& Spec, FGameplayTagContainer TagContainer)
{
	Spec.AppendDynamicAssetTags(TagContainer);
}

TArray<FActiveGameplayEffectHandle> UGAS_EffectBlueprintFunctionLibary::ApplyEffectArrayToTarget(UAbilitySystemComponent* SourceASC, const UGameplayAbility* SourceAbility, UAbilitySystemComponent* TargetASC, TArray<TSubclassOf<UGameplayEffect>> EffectClasses)
{
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectsHandle;
	if (!SourceASC || !TargetASC || EffectClasses.IsEmpty())
	{
		return ActiveGameplayEffectsHandle;
	}

	for (TSubclassOf<UGameplayEffect> GameplayEffectClass : EffectClasses)
	{
		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		EffectContext.SetAbility(SourceAbility);
		FGameplayEffectSpecHandle NewHandle = SourceASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);

		if (!NewHandle.IsValid())
		{
			continue;
		}

		FGameplayEffectSpec& EffectSpec = *NewHandle.Data.Get();

		FActiveGameplayEffectHandle AppliedEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(EffectSpec, TargetASC);

		const UGameplayEffect* EffectDef = EffectSpec.Def;
		if (EffectDef && EffectDef->DurationPolicy != EGameplayEffectDurationType::Instant)
		{
			if (AppliedEffectHandle.IsValid())
			{
				ActiveGameplayEffectsHandle.Add(AppliedEffectHandle);
			}
		}
	}

	return ActiveGameplayEffectsHandle;
}





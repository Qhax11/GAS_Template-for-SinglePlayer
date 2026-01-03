// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_PostureHandler.h"
#include "Gameplay/Components/AC_AbilitySet.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "Gameplay/Attributes/AS_Base.h"


UAC_PostureHandler::UAC_PostureHandler()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_PostureHandler::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AGAS_CharacterBase>(GetOwner());
	if (!OwnerCharacter) 
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_PostureHandler: Owner Character is null in: %s, cannot initalize"), *GetName());
		return;
	}

	// We need to wait init attributes for init of AttributesListenerBase
	if (UAC_AbilitySet* OwnerAbiltySetComp = OwnerCharacter->GetAbilitySetComponent())
	{
		OwnerAbiltySetComp->OnAbilitySetGiven.AddDynamic(this, &UAC_PostureHandler::OnAbilitySetGiven);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_PostureHandler: OwnerAbiltySetComp is null in %s, cannot initialize UAC_PostureHandler."), *this->GetName());
	}
}

void UAC_PostureHandler::OnAbilitySetGiven(const AActor* OwnerActor)
{
	OwnerASC = OwnerCharacter->GetAbilitySystemComponent();
	if (!OwnerASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_PostureHandler: OwnerASC is null in: %s, cannot initalize"), *GetName());
		return;
	}

	BaseAttributes = const_cast<UAS_Base*>(OwnerASC->GetSet<UAS_Base>());
	check(BaseAttributes);

	BaseAttributes->OnHealthChanged.AddDynamic(this, &UAC_PostureHandler::OnHealthChanged);
	BaseAttributes->OnPostureChanged.AddDynamic(this, &UAC_PostureHandler::OnPostureChanged);
	if (BaseAttributes->GetPosture() == BaseAttributes->GetMaxPosture())
	{
		OwnerASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_Attribute_Posture_Full);
	}
}

void UAC_PostureHandler::OnHealthChanged(const FAttributeChangeCallbackData& Data)
{
	if (!OwnerASC || !BaseAttributes)
	{
		return;
	}

	if (OwnerASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead))
	{
		return;
	}

	// Ignore healing
	if (Data.CurrentValue >= Data.OldValue)
	{
		return;
	}

	// HP reached zero → force posture break ONLY
	if (Data.CurrentValue <= 0.f)
	{
		if (BaseAttributes->GetPosture() > 0.f)
		{
			UGameplayEffect* ForcePostureBreakEffect = UGAS_EffectBlueprintFunctionLibary::
				CreateInstantEffectWithModifier(UAS_Base::GetPostureAttribute(), EGameplayModOp::Override, 0.f);

			OwnerASC->ApplyGameplayEffectToSelf(ForcePostureBreakEffect, 1, FGameplayEffectContextHandle());
		}
		return; 
	}

	// HP > 0 → posture reduction
	const float DamageTaken = Data.OldValue - Data.CurrentValue;
	if (DamageTaken <= 0.f)
	{
		return;
	}

	const float PostureReductionAmount = -DamageTaken * PostureDamageMultiplier;
	UGameplayEffect* DecreasePostureEffect = UGAS_EffectBlueprintFunctionLibary::
		CreateInstantEffectWithModifier(UAS_Base::GetPostureAttribute(), EGameplayModOp::Additive, PostureReductionAmount);

	OwnerASC->ApplyGameplayEffectToSelf(DecreasePostureEffect, 1, FGameplayEffectContextHandle());
}

void UAC_PostureHandler::OnPostureChanged(const FAttributeChangeCallbackData& Data)
{
	if (OwnerASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead))
	{
		return;
	}


	if (Data.CurrentValue >= Data.MaxValue)
	{
		if (!OwnerASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Attribute_Posture_Full))
		{
			OwnerASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_Attribute_Posture_Full);
		}
	}
	else
	{
		if (OwnerASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Attribute_Posture_Full))
		{
			OwnerASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_Attribute_Posture_Full, 100);
		}
	}

	if (Data.CurrentValue <= 0)
	{
		if (!OwnerASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Attribute_Posture_Empty))
		{
			OwnerASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_Attribute_Posture_Empty);
		}

		if (!OwnerASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Vulnerable) && !OwnerASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Entity_Character_Hero))
		{
			OwnerASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Vulnerable);
		}
	}
	else
	{
		if (OwnerASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Attribute_Posture_Empty))
		{
			OwnerASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_Attribute_Posture_Empty, 100);
		}

		if (OwnerASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Vulnerable))
		{
			OwnerASC->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Vulnerable, 100);
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(PostureRegenTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(PostureRegenTimerHandle, this, &UAC_PostureHandler::TriggerPostureRegenEffect, PostureRegenDelay, false);
}

void UAC_PostureHandler::TriggerPostureRegenEffect()
{
	if (!OwnerASC)
	{
		return;
	}

	FGameplayEffectSpec PostureRegenSpec;
	bool bIsPostureRegenSpecValid = UGAS_EffectBlueprintFunctionLibary::CreateEffectSpecWithSetByCallerValue(
		PostureRegenSpec,
		OwnerASC,
		PostureRegenGameplayEffect,
		GAS_Tags::TAG_Gameplay_EffectData_SetByCaller_PostureRegenAmount,
		PostureRegenPerTick
	);

	if (!bIsPostureRegenSpecValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_PostureHandler: PostureRegenSpec is null in %s, cannot damage apply"), *GetName());
		return;
	}

	OwnerASC->ApplyGameplayEffectSpecToSelf(PostureRegenSpec);
}

void UAC_PostureHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (PostureRegenTimerHandle.IsValid() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(PostureRegenTimerHandle);
	}
}


// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_MeleeComboManager.h"

UAC_MeleeComboManager::UAC_MeleeComboManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_MeleeComboManager::BeginPlay()
{
	Super::BeginPlay();

	CharacterBase = Cast<AGAS_CharacterBase>(GetOwner());
	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_MeleeComboManager: CharacterBase is null in: %s)"), *GetName());
		return;
	}

	CharacterBaseASC = Cast<UGAS_AbilitySystemComponent>(CharacterBase->GetAbilitySystemComponent());
	if (!CharacterBaseASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_MeleeComboManager: CharacterBaseASC is null in %s, cannot initialize HeroControl."), *GetName());
		return;
	}
}

void UAC_MeleeComboManager::ActivateComboMelee(const UComboPreActivationData* Data)
{
	if (!CharacterBaseASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_MeleeComboManager: CharacterBaseASC is null!"));
		return;
	}

	const FComboAbilityData* ComboAbilityData = ActiveComboChainTracker.GetCurrentCombo();
	if (!ComboAbilityData || !ComboAbilityData->ComboAbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_MeleeComboManager: No valid combo ability at current step!"));
		return;
	}

	// ✅ ÇÖZÜM: Ability'yi önce spec'ten bul veya grant et
	FGameplayAbilitySpec* FoundSpec = nullptr;

	// Mevcut spec'lerde ara
	for (FGameplayAbilitySpec& Spec : CharacterBaseASC->GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->GetClass() == ComboAbilityData->ComboAbilityClass)
		{
			FoundSpec = &Spec;
			break;
		}
	}

	// Bulunamadıysa grant et
	if (!FoundSpec)
	{
		FGameplayAbilitySpecHandle NewHandle = CharacterBaseASC->GiveAbility(
			FGameplayAbilitySpec(ComboAbilityData->ComboAbilityClass, 1, INDEX_NONE, CharacterBase)
		);
		FoundSpec = CharacterBaseASC->FindAbilitySpecFromHandle(NewHandle);
	}

	if (!FoundSpec)
	{
		UE_LOG(LogTemp, Error, TEXT("UAC_MeleeComboManager: Could not find or create ability spec!"));
		return;
	}

	// ✅ ÇÖZÜM: Data'yı EventData ile gönder, CDO'ya set etme!
	FGameplayEventData EventData;
	EventData.EventTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_ComboMelee;
	EventData.OptionalObject = Data; // Instance'da okunacak
	EventData.Instigator = CharacterBase;
	EventData.Target = CharacterBase;

	if (Data)
	{
		UE_LOG(LogTemp, Log, TEXT("UAC_MeleeComboManager: Sending PreActivation data - Loc: %s, Rot: %s"),
			*Data->MotionWarpingLocation.ToString(),
			*Data->MotionWarpingRotation.ToString());
	}

	// ✅ Ability'yi trigger et
	CharacterBaseASC->TriggerAbilityFromGameplayEvent(
		FoundSpec->Handle,
		CharacterBaseASC->AbilityActorInfo.Get(),
		EventData.EventTag,
		&EventData,
		*CharacterBaseASC
	);
}

void UAC_MeleeComboManager::OnComboAbilityActivated(UGA_ComboMeleeAttack* Instance)
{
	if (!Instance) return;

	ActiveComboChainTracker.CurrentAbilityInstance = Instance;

	Instance->OnAbilityEnded.RemoveAll(this);
	Instance->OnAbilityEnded.AddUObject(this, &UAC_MeleeComboManager::OnComboAbilityEnd);

	UE_LOG(LogTemp, Log, TEXT("UAC_MeleeComboManager: Combo ability activated - %s"),*Instance->GetClass()->GetName())
}

void UAC_MeleeComboManager::OnComboAbilityEnd(const FCustomAbilityEndedData& Data)
{
	if (Data.AbilityThatEnded)
	{
		Data.AbilityThatEnded->OnAbilityEnded.RemoveAll(this);
	}
}

void UAC_MeleeComboManager::StopCombo()
{
	CancelComboAbilities();
	ActiveComboChainTracker.Reset();
	OnComboEnded.Broadcast();
}

void UAC_MeleeComboManager::CancelComboAbilities()
{
	if (!CharacterBaseASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAC_MeleeComboManager: CharacterBaseASC is null in: %s"), *GetName());
		return;
	}

	FGameplayTagContainer CancelTags;
	CancelTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Combat_Attack_MeleeCombo);

	CharacterBaseASC->CancelAbilities(&CancelTags);
}

void UAC_MeleeComboManager::ClearComboChain()
{
	ActiveComboChainTracker.CurrentAbilityInstance = nullptr;
	ActiveComboChainTracker.Reset();
}



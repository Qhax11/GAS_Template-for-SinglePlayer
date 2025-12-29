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
		UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s)"), *GetName());
		return;
	}

	CharacterBaseASC = Cast<UGAS_AbilitySystemComponent>(CharacterBase->GetAbilitySystemComponent());
	if (!CharacterBaseASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterBaseASC is null in %s, cannot initialize HeroControl."), *GetName());
		return;
	}
}

void UAC_MeleeComboManager::ActivateComboMelee(const UComboPreActivationData* Data)
{
	if (!CharacterBaseASC || !Data)
		return;

	const FComboAbilityData* ComboAbilityData = ActiveComboChainTracker.GetCurrentCombo();
	if (!ComboAbilityData || !ComboAbilityData->ComboAbilityClass)
		return;

	// 1) event data hazırla
	FGameplayEventData EventData;
	EventData.EventTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_ComboMelee;
	EventData.OptionalObject = Data;
	EventData.Instigator = CharacterBase;
	EventData.Target = CharacterBase;

	// 2) EVENT DISPATCH (asıl tetik bu)
	CharacterBaseASC->HandleGameplayEvent(EventData.EventTag, &EventData);

	// 3) instance yakalamak istiyorsan:
	// InstancedPerExecution ise "hemen sonra instance döndürmek" garanti değil.
	// Bu yüzden tracking'i Ability'nin kendisinden broadcast et.
	return;
}

void UAC_MeleeComboManager::OnComboAbilityActivated(UGA_ComboMeleeAttack* Instance)
{
	if (!Instance) return;

	ActiveComboChainTracker.CurrentAbilityInstance = Instance;

	Instance->OnAbilityEnded.RemoveAll(this);
	Instance->OnAbilityEnded.AddUObject(this, &UAC_MeleeComboManager::OnComboAbilityEnd);
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
		UE_LOG(LogTemp, Warning, TEXT("CharacterBaseASC is null in: %s"), *GetName());
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



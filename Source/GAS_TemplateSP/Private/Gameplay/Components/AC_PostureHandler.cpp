// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_PostureHandler.h"
#include "Gameplay/Components/AC_AbilitySet.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"


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
		UE_LOG(LogTemp, Warning, TEXT("Owner Character is null in: %s, cannot initalize"), *GetName());
		return;
	}

	// We need to wait init attributes for init of AttributesListenerBase
	if (UAC_AbilitySet* OwnerAbiltySetComp = OwnerCharacter->GetAbilitySetComponent())
	{
		OwnerAbiltySetComp->OnAbilitySetGiven.AddDynamic(this, &UAC_PostureHandler::OnAbilitySetGiven);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerAbiltySetComp is null in %s, cannot initialize UAC_PostureHandler."), *this->GetName());
	}
}

void UAC_PostureHandler::OnAbilitySetGiven(const AActor* OwnerActor)
{
	OwnerASC = OwnerCharacter->GetAbilitySystemComponent();
	if (!OwnerASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerASC is null in: %s, cannot initalize"), *GetName());
		return;
	}

	if (UAS_Base* BaseAttributes = const_cast<UAS_Base*>(OwnerASC->GetSet<UAS_Base>()))
	{
		BaseAttributes->OnHealthChanged.AddDynamic(this, &UAC_PostureHandler::OnHealthChanged);
		BaseAttributes->OnPostureChanged.AddDynamic(this, &UAC_PostureHandler::OnPostureChanged);
	}

	UAC_TagDelegates* OwnerCharacterTagDelegatesComp = OwnerCharacter->GetTagDelegatesComponent();
	if (!OwnerCharacterTagDelegatesComp)
	{
		return;
	}

	OwnerCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback, EListenMode::OnAdded).BindDynamic(this, &UAC_PostureHandler::OnKnocbackTagAdded);
	OwnerCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Dash, EListenMode::OnAdded).BindDynamic(this, &UAC_PostureHandler::OnDashTagAdded);
}

void UAC_PostureHandler::OnHealthChanged(const FAttributeChangeCallbackData& Data)
{

}

void UAC_PostureHandler::OnPostureChanged(const FAttributeChangeCallbackData& Data)
{
	if (Data.CurrentValue <= 0) 
	{
		OwnerASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_Vulnerable);
	}
}

void UAC_PostureHandler::OnKnocbackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{

}

void UAC_PostureHandler::OnDashTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{

}





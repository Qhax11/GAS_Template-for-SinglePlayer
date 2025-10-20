// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_FootstepBase.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Gameplay/Tags/GAS_Tags.h"

UAC_FootstepBase::UAC_FootstepBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_FootstepBase::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AGAS_CharacterBase>(GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Character is null in: %s, cannot initalize"), *GetName());
		return;
	}

	OwnerASC = OwnerCharacter->GetAbilitySystemComponent();
	if (!OwnerASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerASC is null in: %s, cannot initalize"), *GetName());
		return;
	}

	OwnerASC->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UAC_FootstepBase::OnGameplayEvent));

}

void UAC_FootstepBase::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) const
{
	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.EffectCauser = OwnerCharacter;
	OwnerASC->ExecuteGameplayCue(GAS_Tags::TAG_GameplayCue_Sound_PlayOnSource_Footstep, GameplayCueParameters);
}

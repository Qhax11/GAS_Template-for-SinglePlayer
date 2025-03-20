// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/GameplayTag/AC_TagDispatcher.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"


UAC_TagDispatcher::UAC_TagDispatcher()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_TagDispatcher::BeginPlay()
{
	Super::BeginPlay();

	if (UAC_TagDelegates* TagDelegatesComponent = GetOwner()->GetComponentByClass<UAC_TagDelegates>())
	{
		TagDelegatesComponent->RegisterDelegateForTags(ListeningTags, EListenMode::OnAdded).BindDynamic(this, &UAC_TagDispatcher::OnTagAdded);
		TagDelegatesComponent->RegisterDelegateForTags(ListeningTags, EListenMode::OnRemoved).BindDynamic(this, &UAC_TagDispatcher::OnTagRemoved);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TagDelegatesComponent is null in %s, cannot listen tags."), *this->GetName());
	}
}

void UAC_TagDispatcher::OnTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OnGameplayTagAdded.Broadcast(AbilitySystemComponent, Tag);
}

void UAC_TagDispatcher::OnTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	OnGameplayTagRemoved.Broadcast(AbilitySystemComponent, Tag);
}







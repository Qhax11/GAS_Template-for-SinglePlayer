// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Services/BTService_AddRemoveGameplayTag.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"

UBTService_AddRemoveGameplayTag::UBTService_AddRemoveGameplayTag()
{
    bNotifyBecomeRelevant = true;
    bNotifyCeaseRelevant = true;
    bNotifyTick = false;
}

void UBTService_AddRemoveGameplayTag::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnBecomeRelevant(OwnerComp, NodeMemory);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return;
    }

    FName LastNode = BlackboardComp->GetValueAsName(CurrentNodeKey.SelectedKeyName);

    if (DefinedNodeName != LastNode)
    {
        OwnerCharacter = Cast<AGAS_CharacterBase>(BlackboardComp->GetValueAsObject(ActorKey.SelectedKeyName));
        if (OwnerCharacter)
        {
            OwnerCharacter->AddGameplayTagsIfNotExist(TagsToAdd);
            OwnerCharacter->RemoveGameplayTagsIfExist(TagsToRemoved);
        }

        BlackboardComp->SetValueAsName(CurrentNodeKey.SelectedKeyName, DefinedNodeName);
    }
}



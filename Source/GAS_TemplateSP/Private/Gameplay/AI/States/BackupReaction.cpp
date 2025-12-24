// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/BackupReaction.h"

UBackupReaction::UBackupReaction()
{
	StateTag = GAS_Tags::TAG_AI_State_BackupReaction;
}

void UBackupReaction::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter(EnterPayload);

	UE_LOG(LogTemp, Warning, TEXT("UBackupReaction::OnEnter called"));
}

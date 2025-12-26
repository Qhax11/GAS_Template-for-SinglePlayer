// Qhax's GAS Template for 2D SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroInteraction.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_AbilityInputBinding.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroAttributesListener.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroRespawn.h"
#include "Gameplay/Actors/Characters/Heroes/Components/SC_HeroShadowController.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroGameplayCamera.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Actors/Characters/Heroes/Components/Listener/AC_HeroTagListener.h"
#include "Gameplay/Actors/Characters/Heroes/Components/Listener/AC_HeroEnemyAttackListener.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroAbilityBuffer.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroAimAssist.h"
#include "Gameplay/Components/AC_AbilitySet.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AGAS_HeroBase::AGAS_HeroBase(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer
        .SetDefaultSubobjectClass<UAC_HeroAttributesListener>(TEXT("AttributesListenerBase")) 
        .SetDefaultSubobjectClass<UAC_HeroRespawn>(TEXT("RespawnBase")))
{
    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character 
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    HeroControlComponent = CreateDefaultSubobject<UAC_HeroControl>(TEXT("HeroControlComponent"));
    HeroInteractionComponent = CreateDefaultSubobject<UAC_HeroInteraction>(TEXT("HeroInteractionComponent"));
    AbilityInputBindingComponent = CreateDefaultSubobject<UAC_AbilityInputBinding>(TEXT("AbilityInputBindingComponent"));
    TargetLockSystemComponent = CreateDefaultSubobject<UAC_TargetLockSystem>(TEXT("TargetLockSystemComponent"));
    HeroMeleeComboManagerComponent = CreateDefaultSubobject<UAC_HeroMeleeComboManager>(TEXT("HeroMeleeComboManagerComponent"));
    HeroMovementListenerComponent = CreateDefaultSubobject<UAC_HeroMovementListener>(TEXT("HeroMovementListenerComponent"));
    HeroGameplayCameraComponent = CreateDefaultSubobject<UAC_HeroGameplayCamera>(TEXT("HeroGameplayCameraComponent"));
    HeroTagListenerComponent = CreateDefaultSubobject<UAC_HeroTagListener>(TEXT("HeroTagListenerComponent"));
    HeroEnemyAttackListenerComponent = CreateDefaultSubobject<UAC_HeroEnemyAttackListener>(TEXT("HeroEnemyAttackListenerComponent"));
    HeroAbilityBufferComponent = CreateDefaultSubobject<UAC_HeroAbilityBuffer>(TEXT("HeroAbilityBufferComponent"));
    HeroAimAssistComponent = CreateDefaultSubobject<UAC_HeroAimAssist>(TEXT("HeroAimAssistComponent"));

    HeroShadowControllerComponent = CreateDefaultSubobject<USC_HeroShadowController>(TEXT("HeroShadowControllerComponent"));
    HeroShadowControllerComponent->SetupAttachment(RootComponent);
}

void AGAS_HeroBase::BeginPlay()
{
    Super::BeginPlay();

    //Adding Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(HeroInputMappingContext, 0);
        }
    }
}

void AGAS_HeroBase::BrodcastCharacterSpawn()
{
    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnHeroSpawn.Broadcast(FHeroSpawnData(this, GetAbilitySystemComponent()));
    }
}













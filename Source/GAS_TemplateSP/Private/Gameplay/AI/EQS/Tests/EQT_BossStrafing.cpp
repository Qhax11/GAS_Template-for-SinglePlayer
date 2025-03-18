// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/EQS/Tests/EQT_BossStrafing.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "Gameplay/AI/Controllers/AIControllerBoss.h"


UEQT_BossStrafing::UEQT_BossStrafing()
{
    // EQS Query'nin neyle çalışacağını belirtiyoruz
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
    SetWorkOnFloatValues(true);
}

void UEQT_BossStrafing::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* Owner = QueryInstance.Owner.Get();
    if (!Owner)
    {
        return;
    }

    AAIControllerBoss* AIBossController = Cast<AAIControllerBoss>(Owner);
    if (!AIBossController)
    {
        return;
    }

    APawn* Boss = AIBossController->GetPawn();
    if (!Boss)
    {
        return;
    }

    ACharacter* Player = Cast<ACharacter>(AIBossController->TargetActor);
    if (!Player)
    {
        return;
    }

    FVector BossLocation = Boss->GetActorLocation();
    FVector BossForward = Boss->GetActorForwardVector();

    FVector PlayerLocation = AIBossController->TargetActor->GetActorLocation();
    FVector PlayerForward = AIBossController->TargetActor->GetActorForwardVector();
    FVector PlayerVelocity = AIBossController->TargetActor->GetVelocity();
    PlayerVelocity.Normalize(); // Normalize edelim ki yön vektörü olsun

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        FVector QueryPoint = GetItemLocation(QueryInstance, It.GetIndex());

        // 1️⃣ **Sağ/Sol Belirleme (Boss'un Forward'ına Göre)**
        FVector ToPoint = QueryPoint - BossLocation;

        // Eğer ToPoint sıfır vektörse işlemi atla (hata önleme)
        if (ToPoint.IsNearlyZero())
        {
            continue;
        }

        // **ToPoint'i normalize et ve tekrar tekrar GetSafeNormal() çağırma**
        FVector ToPointNormalized = ToPoint.GetSafeNormal();

        float CrossZ = FVector::CrossProduct(BossForward, ToPointNormalized).Z;
        bool bIsRight = (CrossZ > 0.0f);  // Pozitifse sağ, negatifse sol

        // 2️⃣ **Açıya Göre Skorlama (Dot Product ile)**
        float AngleScore = FVector::DotProduct(BossForward, ToPointNormalized);

        // Ön taraf kötü, sağ/sol uç noktalar iyi (Strafing için)
        float StrafingScore = 1.0f - FMath::Abs(AngleScore);

        // 3️⃣ **Ön Tarafa Gitmeyi Önleme**
        if (AngleScore > 0.8f)  // Eğer nokta boss'un tam önündeyse cezalandır
        {
            StrafingScore *= 0.5f;  // Skoru düşür
        }

        // 4️⃣ **EQS Skorunu Set Et**
        It.SetScore(EEnvTestPurpose::Score, EEnvTestFilterType::Range, StrafingScore, 0.0f, 1.0f);
    }
}

// Qhax's GAS Template for SinglePlayer


#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"

void US_SpawnDelegates::RegisterEnemy(const FEnemySpawnData& SpawnData)
{
    if (!SpawnData.Character) return;

    // 1. Listeye Ekle (Duplicate kontrolü yaparak)
    // Not: Struct içinde == operatörü yoksa pointer kontrolü yapacaðýz, 
    // basitlik için direkt ekleyelim, ama Remove iþleminde Character pointer'ýna bakacaðýz.
    AliveEnemies.Add(SpawnData);

    // 2. Haber Ver (Broadcast) - O an dinleyen varsa duyar (Örn: UI)
    if (OnEnemySpawn.IsBound())
    {
        OnEnemySpawn.Broadcast(SpawnData);
    }
}

void US_SpawnDelegates::UnregisterEnemy(const FCharacterDeSpawnData& DeSpawnData)
{
    // Listeden bu karakteri bul ve sil
    // Lambda ile Character pointer eþleþmesine bakýyoruz
    AliveEnemies.RemoveAll([&](const FEnemySpawnData& Element)
        {
            return Element.Character == DeSpawnData.Character;
        });

    // Ölüm haberini yay
    if (OnEnemyDeSpawn.IsBound())
    {
        OnEnemyDeSpawn.Broadcast(DeSpawnData);
    }
}


#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyCharacter.h"
#include "RangedEnemyCharacter.generated.h"


UCLASS()
class ADVANCEDAISYSTEM_API ARangedEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	//EnemyInterfaceFunction
	virtual void GetBattleRange_Implementation(float& OutAttackRadius, float& OutDefendRadius) override;
	virtual void Attack_Implementation() override;
	virtual void EquipWeapon_Implementation() override;
	virtual void UnequipWeapon_Implementation() override;
};

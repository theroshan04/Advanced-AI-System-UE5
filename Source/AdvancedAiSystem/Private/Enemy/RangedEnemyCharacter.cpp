
#include "Enemy/RangedEnemyCharacter.h"

void ARangedEnemyCharacter::GetBattleRange_Implementation(float& OutAttackRadius, float& OutDefendRadius)
{
	OutAttackRadius = 600.0f;
	OutDefendRadius = 600.0f;
}

void ARangedEnemyCharacter::Attack_Implementation()
{
	OnAttackEnd.Broadcast();
}

void ARangedEnemyCharacter::EquipWeapon_Implementation()
{
	bIsWeaponEquipped = true;
	OnWeaponEquipped.Broadcast();
}

void ARangedEnemyCharacter::UnequipWeapon_Implementation()
{
	bIsWeaponEquipped = false;
	OnWeaponUnequipped.Broadcast();
}

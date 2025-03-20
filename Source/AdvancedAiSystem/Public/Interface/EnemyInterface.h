
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enumeration/EnemyEnums.h"
#include "EnemyInterface.generated.h"

class AEnemyCharacter;

UINTERFACE(MinimalAPI,Blueprintable)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class ADVANCEDAISYSTEM_API IEnemyInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyInterface")
	AEnemyCharacter* EnemyRef();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyInterface")
	void OnStealthTakenDown(int SelectedMontageIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyInterface")
	void Patrolling();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyInterface")
	void SetMovementSpeed(EMovementSpeed MovementSpeed, float& OutSpeed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyInterface")
	void GetBattleRange(float& OutAttackRadius, float& OutDefendRadius);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyInterface")
	void Attack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyInterface")
	void ClearFocus();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyInterface")
	void EquipWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyInterface")
	void UnequipWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyInterface")
	void JumpToDestination(FVector Destination);
};

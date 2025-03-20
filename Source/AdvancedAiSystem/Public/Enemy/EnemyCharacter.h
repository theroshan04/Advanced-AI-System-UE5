
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/EnemyInterface.h"
#include "Interface/DamageSystemInterface.h"
#include "Enumeration/EnemyEnums.h"
#include "Enumeration/CommonEnums.h"
#include "Structure/CommonStructs.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyCharacter.generated.h"

class AEnemyAIController;
class UDamageSystemActorComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPatrollingDestinationReached);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponEquipped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUnequipped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class ADVANCEDAISYSTEM_API AEnemyCharacter : public ACharacter, public IEnemyInterface, public IDamageSystemInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	//EnemyInterfaceFunction
	virtual AEnemyCharacter* EnemyRef_Implementation() override;
	virtual void OnStealthTakenDown_Implementation(int SelectedMontageIndex) override;
	virtual void Patrolling_Implementation() override;
	virtual void SetMovementSpeed_Implementation(EMovementSpeed MovementSpeed, float& OutSpeed) override;
	virtual void GetBattleRange_Implementation(float& OutAttackRadius, float& OutDefendRadius) override;
	virtual void Attack_Implementation() override;
	virtual void ClearFocus_Implementation() override;
	virtual void EquipWeapon_Implementation() override;
	virtual void UnequipWeapon_Implementation() override;
	virtual void JumpToDestination_Implementation(FVector Destination) override;

	//DamageSystemInterfaceFunction
	virtual float GetCurrentHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;
	virtual float Heal_Implementation(float Amount) override;
	virtual bool TakeDamage_Implementation(FDamageInfo& DamageInfo) override;
	virtual bool IsDead_Implementation() override;
	virtual void DamageResponse_Implementation(EDamageResponse& DamageResponse) override;
	virtual void Death_Implementation() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnPatrollingDestinationReached OnPatrollingDestinationReached;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnWeaponEquipped OnWeaponEquipped;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnWeaponUnequipped OnWeaponUnequipped;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnAttackEnd OnAttackEnd;

private:
	UCapsuleComponent* EnemyCapsule = nullptr;
	USkeletalMeshComponent* EnemyMesh = nullptr;
	UCharacterMovementComponent* EnemyMovementComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DamageSystem", meta = (AllowPrivateAccess = "true"))
	UDamageSystemActorComponent* DamageSystemComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reference", meta = (AllowPrivateAccess = "true"))
	AEnemyAIController* EnemyController = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*>StealthVictimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	bool bIsWeaponEquipped;
};

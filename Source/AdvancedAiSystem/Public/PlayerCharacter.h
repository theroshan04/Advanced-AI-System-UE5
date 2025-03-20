
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/DamageSystemInterface.h"
#include "PlayerCharacter.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class UCharacterMovementComponent;
class UDamageSystemActorComponent;
class AMainPlayerController;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class ADVANCEDAISYSTEM_API APlayerCharacter : public ACharacter, public IDamageSystemInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//DamageSystemInterfaceFunction
	virtual float GetCurrentHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;
	virtual float Heal_Implementation(float Amount) override;
	virtual bool TakeDamage_Implementation(FDamageInfo& DamageInfo) override;
	virtual bool IsDead_Implementation() override;
	virtual void DamageResponse_Implementation(EDamageResponse& DamageResponse) override;
	virtual void Death_Implementation() override;

protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StealthAttack();

private:
	AMainPlayerController* PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Reference", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Reference", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* PlayerCapsule;

	UPROPERTY(BlueprintReadOnly, Category = "Reference", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* PlayerMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DamageSysytem", meta = (AllowPrivateAccess = "true"))
	UDamageSystemActorComponent* DamageSystemComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* StealthAttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

};

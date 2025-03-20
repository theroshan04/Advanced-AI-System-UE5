
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class AEnemyCharacter;
class UCharacterMovementComponent;

UCLASS()
class ADVANCEDAISYSTEM_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:

	UPROPERTY()
	AEnemyCharacter* EnemyCharacter = nullptr;

	UPROPERTY()
	UCharacterMovementComponent* EnemyMovementComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GroundMovement", meta = (AllowPrivateAccess = "true"))
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GroundMovement", meta = (AllowPrivateAccess = "true"))
	float Direction;


};

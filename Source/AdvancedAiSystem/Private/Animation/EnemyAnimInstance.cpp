
#include "Animation/EnemyAnimInstance.h"
#include "Enemy/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (!(TryGetPawnOwner())) return;
	EnemyCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());
	EnemyMovementComponent = EnemyCharacter->FindComponentByClass<UCharacterMovementComponent>();
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!EnemyMovementComponent || !EnemyCharacter) return;
	FVector Velocity = EnemyMovementComponent->GetLastUpdateVelocity();
	FRotator Rotation = EnemyMovementComponent->GetLastUpdateRotation();
	GroundSpeed = Velocity.Size2D();
	Direction = CalculateDirection(Velocity, Rotation);

}


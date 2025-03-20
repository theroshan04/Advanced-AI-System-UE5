
#include "Animation/PlayerAnimInstance.h"
#include "PlayerCharacter.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!TryGetPawnOwner()) return;
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!PlayerCharacter) return;
	FVector PlayerVelocity = PlayerCharacter->GetVelocity();
	FRotator PlayerRotation = PlayerCharacter->GetActorRotation();
	Speed = PlayerVelocity.Size();
	Direction = CalculateDirection(PlayerVelocity, PlayerRotation);
}

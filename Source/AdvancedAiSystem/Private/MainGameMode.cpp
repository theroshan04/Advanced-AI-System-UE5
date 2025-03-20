
#include "MainGameMode.h"
#include "PlayerCharacter.h"
#include "MainPlayerController.h"

AMainGameMode::AMainGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();
}

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DefaultGameMode.generated.h"

/**
 *
 */
UCLASS()
class ECHOESOFTIME_API ADefaultGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADefaultGameMode();

	// Respawn the player to the closest PlayerStart
	void RespawnPlayer(APlayerController* PlayerController);

	// Choose a specific PlayerStart based on player index
	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UFUNCTION(BlueprintCallable)
	AActor* WhichPlayerStart(AController* Player);

};

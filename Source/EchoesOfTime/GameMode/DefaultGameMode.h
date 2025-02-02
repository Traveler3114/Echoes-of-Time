// Fill out your copyright notice in the Description page of Project Settings.

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
private:
	APlayerController* PausingPlayer = nullptr;
public:
	ADefaultGameMode();
	void RespawnPlayer(APlayerController* PlayerController);
	AActor* ChoosePlayerStart_Implementation(AController* Player);
}; 

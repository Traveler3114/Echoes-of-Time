// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameController.generated.h"

/**
 * 
 */
UCLASS()
class ECHOESOFTIME_API AGameController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	void HighPingWarning();
	void StopHighPingWarning();
	void CheckPing(float DeltaTime);
private:
	UPROPERTY()
	class ACharacterHUD* CharacterHUD;

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	float HighPingRunningTime = 0.f;

	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5.f;

	float PingAnimationRunningTime = 0.f;

	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 20.f;

	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 50.f;

	
};

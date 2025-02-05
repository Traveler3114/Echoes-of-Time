// Fill out your copyright notice in the Description page of Project Settings.


#include "GameController.h"
#include "EchoesOfTime/Widget/HUD/CharacterHUD.h"
#include "EchoesOfTime/Widget/HUD/CharacterOverlay.h"
#include "Components/Image.h"
#include "GameFramework/PlayerState.h"

void AGameController::BeginPlay()
{
	CharacterHUD = CharacterHUD == nullptr ? Cast<ACharacterHUD>(GetHUD()) : CharacterHUD;
	if (CharacterHUD)
	{
		if (CharacterHUD->CharacterOverlay == nullptr) CharacterHUD->AddCharacterOverlay();
	}
}

void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckPing(DeltaTime);
}

void AGameController::CheckPing(float DeltaTime)
{
	APlayerState* CurrentPlayerState = GetPlayerState<APlayerState>();
	HighPingRunningTime += DeltaTime;

	if (HighPingRunningTime > CheckPingFrequency)
	{
		CurrentPlayerState = CurrentPlayerState == nullptr ? GetPlayerState<APlayerState>() : CurrentPlayerState;
		if (CurrentPlayerState)
		{
			if (CurrentPlayerState->GetCompressedPing() * 4 > HighPingThreshold)
			{
				HighPingWarning();
				PingAnimationRunningTime = 0.f;
			}
		}
		HighPingRunningTime = 0.f;
	}

	bool bHighPingAnimationPlaying =
		CharacterHUD &&
		CharacterHUD->CharacterOverlay &&
		CharacterHUD->CharacterOverlay->HighPingAnimation &&
		CharacterHUD->CharacterOverlay->IsAnimationPlaying(CharacterHUD->CharacterOverlay->HighPingAnimation);

	if (bHighPingAnimationPlaying) {
		PingAnimationRunningTime += DeltaTime;
		if (PingAnimationRunningTime > HighPingDuration) {
			StopHighPingWarning();
		}
	}
}


void AGameController::HighPingWarning()
{
	CharacterHUD = CharacterHUD == nullptr ? Cast<ACharacterHUD>(GetHUD()) : CharacterHUD;
	bool bHUDValid = CharacterHUD &&
		CharacterHUD->CharacterOverlay &&
		CharacterHUD->CharacterOverlay->HighPingImage &&
		CharacterHUD->CharacterOverlay->HighPingAnimation;
	if (bHUDValid) {
		CharacterHUD->CharacterOverlay->HighPingImage -> SetOpacity(1.f);
		CharacterHUD->CharacterOverlay->PlayAnimation(CharacterHUD->CharacterOverlay->HighPingAnimation,0.f,5);
	}
}

void AGameController::StopHighPingWarning()
{
	CharacterHUD = CharacterHUD == nullptr ? Cast<ACharacterHUD>(GetHUD()) : CharacterHUD;
	bool bHUDValid = CharacterHUD &&
		CharacterHUD->CharacterOverlay &&
		CharacterHUD->CharacterOverlay->HighPingImage &&
		CharacterHUD->CharacterOverlay->HighPingAnimation;
	if (bHUDValid) {
		CharacterHUD->CharacterOverlay->HighPingImage->SetOpacity(0.f);
		if (CharacterHUD->CharacterOverlay->IsAnimationPlaying(CharacterHUD->CharacterOverlay->HighPingAnimation)) {
			CharacterHUD->CharacterOverlay->StopAnimation(CharacterHUD->CharacterOverlay->HighPingAnimation);
		}
	}
}



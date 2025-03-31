#include "DefaultGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EchoesOfTime/MyGameInstance.h"
#include "EngineUtils.h"


ADefaultGameMode::ADefaultGameMode()
{
    bUseSeamlessTravel = true;
}




//void ADefaultGameMode::RespawnPlayer(APlayerController* PlayerController)
//{
//    if (!PlayerController) return;
//
//    // Get the player's character
//    ACharacter* PlayerCharacter = PlayerController->GetCharacter();
//    if (!PlayerCharacter) return;
//
//    // Get the player's current location
//    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
//
//    // Find the closest PlayerStart
//    APlayerStart* ClosestPlayerStart = nullptr;
//    float ClosestDistance = FLT_MAX;
//
//    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
//    {
//        APlayerStart* PlayerStart = *It;
//        if (PlayerStart)
//        {
//            float Distance = FVector::Dist(PlayerLocation, PlayerStart->GetActorLocation());
//            if (Distance < ClosestDistance)
//            {
//                ClosestDistance = Distance;
//                ClosestPlayerStart = PlayerStart;
//            }
//        }
//    }
//
//    // If a PlayerStart was found, respawn the player there
//    if (ClosestPlayerStart)
//    {
//        FVector RespawnLocation = ClosestPlayerStart->GetActorLocation();
//        FRotator RespawnRotation = ClosestPlayerStart->GetActorRotation();
//        PlayerCharacter->SetActorLocationAndRotation(RespawnLocation, RespawnRotation);
//
//        // Optionally reset health, ammo, etc.
//    }
//}


//NE DIRAJ OVAJ KOD NIKAD!!!!!!!!!!!!!!!!!!!
//AActor* ADefaultGameMode::ChoosePlayerStart_Implementation(AController* Player)
//{
//    if (!Player || !Player->PlayerState)
//    {
//        return Super::ChoosePlayerStart_Implementation(Player);
//    }
//
//    // Get the player's index in the PlayerArray
//    AGameStateBase* LocalGameState = GetGameState<AGameStateBase>();
//    if (!LocalGameState)
//    {
//        return Super::ChoosePlayerStart_Implementation(Player);
//    }
//
//    int32 PlayerIndex = LocalGameState->PlayerArray.IndexOfByKey(Player->PlayerState);
//    UE_LOG(LogTemp, Warning, TEXT("Player Index: %d"), PlayerIndex);
//
//    // Determine desired PlayerStart tag
//    FString DesiredTag = (PlayerIndex == 0) ? TEXT("PlayerStart1") : TEXT("PlayerStart2");
//
//    // Get all PlayerStart actors
//    TArray<AActor*> PlayerStarts;
//    UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
//
//    // Find the PlayerStart with the correct tag
//    for (AActor* Start : PlayerStarts)
//    {
//        APlayerStart* PlayerStart = Cast<APlayerStart>(Start);
//        if (PlayerStart && PlayerStart->PlayerStartTag == FName(DesiredTag))
//        {
//            UE_LOG(LogTemp, Warning, TEXT("Spawning Player %d at %s"), PlayerIndex, *DesiredTag);
//            return PlayerStart;
//            
//        }
//    }
//
//    // Fallback to default PlayerStart if no match is found
//    UE_LOG(LogTemp, Warning, TEXT("No matching PlayerStart found for Player %d"), PlayerIndex);
//    return Super::ChoosePlayerStart_Implementation(Player);
//}

AActor* ADefaultGameMode::WhichPlayerStart(AController* Player)
{
    if (!Player || !Player->PlayerState)
    {
        return ChoosePlayerStart_Implementation(Player);
    }

    // Get the player's index in the PlayerArray
    AGameStateBase* LocalGameState = GetGameState<AGameStateBase>();
    if (!LocalGameState)
    {
        return ChoosePlayerStart_Implementation(Player);
    }

    int32 PlayerIndex = LocalGameState->PlayerArray.IndexOfByKey(Player->PlayerState);
    UE_LOG(LogTemp, Warning, TEXT("Player Index: %d"), PlayerIndex);

    // Determine desired PlayerStart tag
    FString DesiredTag = (PlayerIndex == 0) ? TEXT("PlayerStart1") : TEXT("PlayerStart2");

    // Get all PlayerStart actors
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

    // Find the PlayerStart with the correct tag
    for (AActor* Start : PlayerStarts)
    {
        APlayerStart* PlayerStart = Cast<APlayerStart>(Start);
        if (PlayerStart && PlayerStart->PlayerStartTag == FName(DesiredTag))
        {
            UE_LOG(LogTemp, Warning, TEXT("Spawning Player %d at %s"), PlayerIndex, *DesiredTag);
            return PlayerStart;

        }
    }

    // Fallback to default PlayerStart if no match is found
    UE_LOG(LogTemp, Warning, TEXT("No matching PlayerStart found for Player %d"), PlayerIndex);
    return ChoosePlayerStart_Implementation(Player);
}



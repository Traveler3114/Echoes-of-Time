#include "TimeManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"  // For GetPlayerCharacter()
#include "EchoesOfTime/Character/MyCharacter.h"
#include "Engine/World.h"  // For TActorIterator
#include "EngineUtils.h"

UTimeManager::UTimeManager()
{
    PrimaryComponentTick.bCanEverTick = true;
    bIsPlayerStart1 = true; // Default to PlayerStart1
}


void UTimeManager::BeginPlay()
{
    Super::BeginPlay();

    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        // Directly assign them or store them in an array
        if (!PlayerStart1)
        {
            PlayerStart1 = *It;
        }
        else if (!PlayerStart2)
        {
            PlayerStart2 = *It;
        }
    }

    if (!PlayerStart1 || !PlayerStart2)
    {
        return;
    }
    if (!Character) {
        return;
    }

    FVector CurrentLocation = Character->GetActorLocation();

    // Check which PlayerStart is closer and initialize the map state
    float DistanceToPlayerStart1 = FVector::Dist(CurrentLocation, PlayerStart1->GetActorLocation());
    float DistanceToPlayerStart2 = FVector::Dist(CurrentLocation, PlayerStart2->GetActorLocation());

    if (DistanceToPlayerStart1 < DistanceToPlayerStart2)
    {
        bIsPlayerStart1 = true;
    }
    else
    {
        bIsPlayerStart1 = false;
    }

}

void UTimeManager::SwitchMap(AMyCharacter* NewCharacter)
{
    if (!PlayerStart1 || !PlayerStart2)
    {
        return;
    }

    FVector CurrentPlayerLocation = NewCharacter->GetActorLocation();
    FVector NewLocation;
    FVector OffsetInMap;

    if (bIsPlayerStart1) // Currently at PlayerStart1
    {
        if (!PlayerStart1)
        {
            return;
        }
        OffsetInMap = CurrentPlayerLocation - PlayerStart1->GetActorLocation();
        NewLocation = PlayerStart2->GetActorLocation() + OffsetInMap;
        bIsPlayerStart1 = false; // Switch to FutureMap
    }
    else // Currently at PlayerStart2
    {
        if (!PlayerStart2)
        {
            return;
        }
        OffsetInMap = CurrentPlayerLocation - PlayerStart2->GetActorLocation();
        NewLocation = PlayerStart1->GetActorLocation() + OffsetInMap;
        bIsPlayerStart1 = true; // Switch to PresentMap
    }

    // Teleport the player
    NewCharacter->SetActorLocation(NewLocation);
}



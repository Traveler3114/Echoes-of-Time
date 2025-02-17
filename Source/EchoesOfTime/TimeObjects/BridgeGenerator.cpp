#include "BridgeGenerator.h"
#include "EchoesOfTime/TimeObjects/Bridge.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ABridgeGenerator::ABridgeGenerator()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if not needed.
    PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABridgeGenerator::BeginPlay()
{
    Super::BeginPlay();
    // Automatically generate the bridge when the game starts
    if(HasAuthority()) GenerateBridge();
}






void ABridgeGenerator::GenerateBridge_Implementation()
{
    if (!BridgeActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("BridgeActorClass is not set on BridgeGenerator!"));
        return;
    }


    FVector CurrentLocation = GetActorLocation(); // Start at the generator's location
    FVector CurrentFutureLocation = FutureStartLocation;
    FRotator Rotation = FRotator::ZeroRotator;

    // Loop through the number of platforms
    for (int32 i = 0; i < NumRows; ++i)
    {
        // For each platform, create the rows
        for (int32 j = 0; j < NumColumns; ++j)
        {
            // Calculate the location for the current row's platform
            FVector Location = CurrentLocation + FVector(j * RowSpacing, 0, 0); // Moving in X direction for rows
            FVector FutureLocation = CurrentFutureLocation + FVector(j * RowSpacing, 0, 0); // Moving in X direction for rows


            SpawnBridgeActor(Location,FutureLocation);
            //SpawnBridgeActor(FutureLocation);
        }

        // Move the current location for the next row of platforms (along Y axis)
        CurrentLocation.Y += PlatformSpacing;
        CurrentFutureLocation.Y += PlatformSpacing;
    }
}

void ABridgeGenerator::SpawnBridgeActor_Implementation(FVector Location, FVector FutureLocation)
{
    //float DesiredZScale = 0.25f; // Replace with your fixed Z-scale value
    //FVector RandomScale = FVector(1, 1, DesiredZScale);

    // Spawn the bridge actor
    ABridge* BridgeActor = GetWorld()->SpawnActor<ABridge>(BridgeActorClass, Location, FRotator::ZeroRotator);
    if (BridgeActor)
    {
        //BridgeActor->SetActorScale3D(RandomScale);
        BridgeActor->SetActorScale3D(BoxScale);

        // Randomize falling behavior
        BridgeActor->bCanFall = FMath::RandBool();

        if (BridgeActor->bCanFall == false) {
            ABridge* FutureBridgeActor = GetWorld()->SpawnActor<ABridge>(BridgeActorClass, FutureLocation, FRotator::ZeroRotator);
            //BridgeActor2->SetActorScale3D(RandomScale);
            FutureBridgeActor->SetActorScale3D(BoxScale);
        }
    }
}




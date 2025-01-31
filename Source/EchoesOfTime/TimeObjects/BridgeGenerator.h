#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BridgeGenerator.generated.h"



UCLASS()
class ECHOESOFTIME_API ABridgeGenerator : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABridgeGenerator();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    

public:

    // Number of platforms to spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge Settings")
    int32 NumRows = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge Settings")
    int32 NumColumns = 10;

    // Distance between each platform
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge Settings")
    float PlatformSpacing = 256.5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge Settings")
    float RowSpacing = 256.5;

    // Class of the bridge actor to spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge Settings")
    TSubclassOf<class ABridge> BridgeActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge Settings")
     FVector FutureStartLocation = FVector(-15155.000000, 2375.000000, 415.000000);

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge Settings")
     FVector BoxScale = FVector(1, 1, 0.25);

    UFUNCTION(Server, Reliable)
    void GenerateBridge();


    UFUNCTION(Server, Reliable)
    void SpawnBridgeActor(FVector Location, FVector FutureLocation);
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bridge.generated.h"

UCLASS()
class ECHOESOFTIME_API ABridge : public AActor
{
    GENERATED_BODY()

public:
    ABridge();

    UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
    bool bCanFall = false;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* CollisionBox;



    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(Server, Reliable)
    void ServerEnablePhysics();
};

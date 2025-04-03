// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PresentCube.generated.h"

UCLASS()
class ECHOESOFTIME_API APresentCube : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    APresentCube();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ObjectMesh;

    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* CollisionBox;

    UPROPERTY(VisibleAnywhere)
    class UWidgetComponent* WidgetComponent;

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

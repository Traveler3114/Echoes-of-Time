#include "Bridge.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

ABridge::ABridge()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create and configure the static mesh
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    // Create and configure the collision box
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->SetBoxExtent(FVector(50.f)); // Adjust as needed
    CollisionBox->SetCollisionProfileName(TEXT("Trigger"));

    // Bind the overlap event
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABridge::OnOverlapBegin);

    // Ensure physics simulation is initially disabled
    Mesh->SetSimulatePhysics(false);

}

void ABridge::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABridge, bCanFall);
}


void ABridge::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        SetOwner(PlayerController);
    }
}

void ABridge::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABridge::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()) && bCanFall)
    {
        if (HasAuthority())
        {
            Mesh->SetSimulatePhysics(true);
        }
        else
        {
            ServerEnablePhysics();
        }
    }
}

void ABridge::ServerEnablePhysics_Implementation()
{
    if (bCanFall)
    {
        Mesh->SetSimulatePhysics(true);
    }
}


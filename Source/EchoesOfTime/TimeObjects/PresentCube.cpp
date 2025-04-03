#include "PresentCube.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Pawn.h"

// Sets default values
APresentCube::APresentCube()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize CubeMesh or other components
    ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
    RootComponent = ObjectMesh;

    // Enable physics for the mesh
    ObjectMesh->SetSimulatePhysics(true);

    // Initialize the collision box
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->SetBoxExtent(FVector(200.0f, 200.0f, 32.0f)); // Adjust the size as needed
    CollisionBox->SetCollisionProfileName("Trigger");
    CollisionBox->SetGenerateOverlapEvents(true); // Ensure overlap events are generated
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // Bind the overlap events
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APresentCube::OnOverlapBegin);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APresentCube::OnOverlapEnd);

    // Initialize the widget component
    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    WidgetComponent->SetupAttachment(RootComponent);
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComponent->SetDrawSize(FVector2D(200.0f, 100.0f)); // Adjust the size as needed
    WidgetComponent->SetVisibility(false); // Initially hidden
}

void APresentCube::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        APawn* PlayerPawn = Cast<APawn>(OtherActor); // Check if the overlapping actor is a Pawn
        if (PlayerPawn && PlayerPawn->IsPlayerControlled()) // Ensure it's the Player Pawn
        {
            WidgetComponent->SetVisibility(true); // Show the widget
        }
    }
}

void APresentCube::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this)
    {
        APawn* PlayerPawn = Cast<APawn>(OtherActor); // Check if the overlapping actor is a Pawn
        if (PlayerPawn && PlayerPawn->IsPlayerControlled()) // Ensure it's the Player Pawn
        {
            WidgetComponent->SetVisibility(false); // Hide the widget
        }
    }
}

// Called when the game starts or when spawned
void APresentCube::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void APresentCube::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

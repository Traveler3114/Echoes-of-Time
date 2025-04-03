#include "PresentCube.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "EchoesOfTime/Character/MyCharacter.h"

// Sets default values
APresentCube::APresentCube()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize ObjectMesh
    ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
    RootComponent = ObjectMesh;

    // Enable physics for the mesh
    ObjectMesh->SetSimulatePhysics(true);

    // Initialize BoxCollision
    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    BoxCollision->SetupAttachment(RootComponent);
    BoxCollision->SetBoxExtent(FVector(200.0f, 200.0f, 32.0f));
    

    // Initialize Widget
    Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
    Widget->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void APresentCube::BeginPlay()
{
	Super::BeginPlay();
    Widget->SetVisibility(false);
    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APresentCube::OnOverlapBegin);
    BoxCollision->OnComponentEndOverlap.AddDynamic(this, &APresentCube::OnOverlapEnd);
}

// Called every frame
void APresentCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APresentCube::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
    if (Character) {
        Widget->SetVisibility(true);
    }
}

void APresentCube::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
    AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
    if (Character) {
        Widget->SetVisibility(false);
    }
}

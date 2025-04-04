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



    // Initialize Widget
    Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
    Widget->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void APresentCube::BeginPlay()
{
	Super::BeginPlay();
    Widget->SetVisibility(false);
}

// Called every frame
void APresentCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


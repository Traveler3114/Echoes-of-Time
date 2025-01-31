#include "FutureCube.h"
#include "PresentCube.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AFutureCube::AFutureCube()
{
    PrimaryActorTick.bCanEverTick = true;

    ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
    RootComponent = ObjectMesh;

    ObjectMesh->SetSimulatePhysics(true);
}

void AFutureCube::BeginPlay()
{
    Super::BeginPlay();

    if (PresentObject)
    {
        // Get the initial relative position between the PresentObject and FutureCube (we're only interested in the X difference)
        FVector PresentLocation = PresentObject->GetActorLocation();
        FVector FutureLocation = GetActorLocation();


        // Store the initial offset on the X-axis (X difference only)
        InitialXOffset = PresentLocation.X - FutureLocation.X;
    }
}

void AFutureCube::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (Check) {
        Check = false;
    }

    if (PresentObject)
    {
        // Get the current location and velocity of the PresentObject
        FVector PresentLocation = PresentObject->GetActorLocation();
        FVector PresentVelocity = PresentObject->GetVelocity();  // Get the PresentObject's velocity

        // Get the previous location of the PresentObject (to check if it's moving)
        FVector PreviousLocation = PreviousPresentObjectTransform.GetLocation();

        // Check if the PresentObject has moved
        bool bPresentIsMoving = !PresentLocation.Equals(PreviousLocation);

        if (!bPresentIsMoving)
        {
            // If the PresentObject isn't moving, allow the FutureCube to move freely
            // Move the FutureCube independently based on its own physics
            FVector CurrentVelocity = ObjectMesh->GetPhysicsLinearVelocity();
            CurrentVelocity.X += 10.f * DeltaTime;  // Example: move along the X-axis

            ObjectMesh->SetPhysicsLinearVelocity(CurrentVelocity);  // Apply independent velocity to the FutureCube
        }
        else
        {
            // Otherwise, mirror the PresentObject's position and rotation using physics
            FVector FutureLocation = PresentLocation;

            // Apply the stored X offset to maintain the initial relative X difference
            FutureLocation.X = PresentLocation.X - InitialXOffset;

            // Set the FutureCube's location and rotation to match the PresentObject's mirrored movement
            SetActorLocation(FutureLocation);
            SetActorRotation(PresentObject->GetActorRotation());

            // Apply the velocity of the PresentObject to the FutureCube to mirror the physics-based movement
            ObjectMesh->SetPhysicsLinearVelocity(PresentVelocity);  // Apply velocity for mirroring
        }

        // Store the current location as previous for the next tick comparison
        PreviousPresentObjectTransform.SetLocation(PresentLocation);
    }
}




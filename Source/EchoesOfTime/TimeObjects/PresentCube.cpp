// Fill out your copyright notice in the Description page of Project Settings.


#include "PresentCube.h"

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


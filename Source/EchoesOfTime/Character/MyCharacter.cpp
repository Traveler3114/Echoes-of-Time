// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EchoesOfTime/TimeManager/TimeManager.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EchoesOfTime/GameMode/DefaultGameMode.h"

#include "GameFramework/Controller.h"
#include "InputActionValue.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    TimeManager = CreateDefaultSubobject<UTimeManager>(TEXT("TimeManager"));
    TimeManager->SetIsReplicated(true);

    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

    // Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
    // instead of recompiling to adjust them
    GetCharacterMovement()->JumpZVelocity = 500.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 300.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}



//void AMyCharacter::NotifyActorBeginOverlap(AActor* OtherActor) {
//    Super::NotifyActorBeginOverlap(OtherActor);
//    if (OtherActor->ActorHasTag("OutOfBounds")) {
//        ADefaultGameMode* GameMode = Cast<ADefaultGameMode>(GetWorld()->GetAuthGameMode());
//        APlayerController* PlayerController = Cast<APlayerController>(GetController());
//        if (GameMode)
//        {
//            GameMode->RespawnPlayer(PlayerController);  // Pass the PlayerController to the GameMode
//        }
//    }
//}




void AMyCharacter::ServerMapSwitch_Implementation()
{
    if (TimeManager) // If we're the server, just call the function directly
    {
        TimeManager->SwitchMap(this);
    }
}
void AMyCharacter::MapSwitch(const FInputActionValue& Value)
{

    if (bCanSwitchMap)
    {
        if (TimeManager)// If we're the server, just call the function directly
        {
            if (HasAuthority()) {
                TimeManager->SwitchMap(this);
            }
            else {
                ServerMapSwitch();
            }
        }

        // Disable further switches until the button is released
        bCanSwitchMap = false;
    }
}

void AMyCharacter::MapSwitchReleased(const FInputActionValue& Value)
{
    // Allow the map to be switched again when the button is released
    bCanSwitchMap = true;
}






void AMyCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    if (TimeManager) {
        TimeManager->Character = this;
    }
}


// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            // Add the mapping context
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Bind the Move action
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
        // Bind the Look action
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

        EnhancedInputComponent->BindAction(SwitchMapAction, ETriggerEvent::Started, this, &AMyCharacter::MapSwitch); // On press
        EnhancedInputComponent->BindAction(SwitchMapAction, ETriggerEvent::Completed, this, &AMyCharacter::MapSwitchReleased); // On release

        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyCharacter::Jump); // On press
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyCharacter::StopJumping); // On release

        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMyCharacter::StartSprint);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyCharacter::StopSprint);
    }
}






//void AMyCharacter::Move(const FInputActionValue& Value)
//{
//    // input is a Vector2D
//    FVector2D MovementVector = Value.Get<FVector2D>();
//
//    if (Controller != nullptr)
//    {
//        // find out which way is forward
//        const FRotator Rotation = Controller->GetControlRotation();
//        const FRotator YawRotation(0, Rotation.Yaw, 0);
//
//        // get forward vector
//        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
//
//        // get right vector 
//        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
//
//        // add movement 
//        AddMovementInput(ForwardDirection, MovementVector.Y);
//        AddMovementInput(RightDirection, MovementVector.X);
//    }
//}

void AMyCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);

        // Stop sprinting if moving sideways or backward
        if (MovementVector.Y <= 0 || MovementVector.X != 0)
        {
            StopSprint();
        }
    }
}



void AMyCharacter::Look(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}


void AMyCharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = 600.f; // Sprint speed
}

void AMyCharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = 300.f; // Normal speed
}





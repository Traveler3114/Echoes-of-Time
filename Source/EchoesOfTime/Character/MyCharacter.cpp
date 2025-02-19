#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EchoesOfTime/TimeManager/TimeManager.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "InputActionValue.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set default character collision size
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetMesh()->SetOwnerNoSee(false); // Make sure the mesh is visible in first-person

	// Prevent character rotation from affecting the camera
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	// Character movement settings
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create first-person camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh(), FName("head")); // Attach to head bone (adjust as needed)
	FirstPersonCamera->bUsePawnControlRotation = true;

	// Create and replicate TimeManager
	TimeManager = CreateDefaultSubobject<UTimeManager>(TEXT("TimeManager"));
	TimeManager->SetIsReplicated(true);
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (TimeManager) {
		TimeManager->Character = this;
	}
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Setup input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInputComponent->BindAction(SwitchMapAction, ETriggerEvent::Started, this, &AMyCharacter::MapSwitch);
		EnhancedInputComponent->BindAction(SwitchMapAction, ETriggerEvent::Completed, this, &AMyCharacter::MapSwitchReleased);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyCharacter::StopJumping);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMyCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyCharacter::StopSprint);
	}
}

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
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}




void AMyCharacter::ServerMapSwitch_Implementation()
{
	if (TimeManager)
	{
		TimeManager->SwitchMap(this);
	}
}

void AMyCharacter::MapSwitch(const FInputActionValue& Value)
{
	if (bCanSwitchMap)
	{
		if (TimeManager)
		{
			if (HasAuthority())
			{
				TimeManager->SwitchMap(this);
			}
			else
			{
				ServerMapSwitch();
			}
		}
		bCanSwitchMap = false;
	}
}

void AMyCharacter::MapSwitchReleased(const FInputActionValue& Value)
{
	bCanSwitchMap = true;
}



void AMyCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AMyCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

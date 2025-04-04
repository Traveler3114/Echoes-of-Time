#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EchoesOfTime/TimeManager/TimeManager.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set default character collision size
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

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

	// Create and replicate TimeManager
	TimeManager = CreateDefaultSubobject<UTimeManager>(TEXT("TimeManager"));
	TimeManager->SetIsReplicated(true);

	// Initialize sprint state
	bIsSprinting = false;
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
	CameraComponent = FindComponentByClass<UCameraComponent>();
	if (!CameraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraComponent not found!"));
	}
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
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMyCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AMyCharacter::StopCrouching);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMyCharacter::ServerStartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyCharacter::ServerStopSprint);
		EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Started, this, &AMyCharacter::ServerPickup);
		EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Completed, this, &AMyCharacter::ServerDrop);
	}
}


void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector CameraLocation = CameraComponent->GetComponentLocation();
		FRotator CameraRotation = CameraComponent->GetComponentRotation();
		FVector ForwardVector = CameraRotation.Vector();
		FVector TargetLocation = CameraLocation + (ForwardVector * 300.f);

		// Smooth interpolation for network correction
		FVector CurrentLocation = PhysicsHandle->GrabbedComponent->GetComponentLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 10.0f);

		if (PhysicsHandle)
		{
			PhysicsHandle->SetTargetLocationAndRotation(NewLocation, CameraRotation);
		}
	}
}


void AMyCharacter::Pickup()
{
	if (HitActor) {
		UPrimitiveComponent* HitComponent = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());

		PhysicsHandle->GrabComponentAtLocation(
			HitComponent,
			NAME_None,
			HitComponent->GetComponentLocation()
		);
	}
}

void AMyCharacter::ServerPickup_Implementation() 
{
	Pickup();
}

void AMyCharacter::Drop()
{
	if (PhysicsHandle)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

void AMyCharacter::ServerDrop_Implementation()
{
	Drop();
}

void AMyCharacter::StartCrouch()
{
	Crouch();
}

void AMyCharacter::StopCrouching()
{
	UnCrouch();
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
			ServerStopSprint();
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
		if (CameraComponent) {
			ServerCameraRotationUpdate(CameraComponent->GetComponentRotation().Pitch);
		}
	}
}

void AMyCharacter::ServerCameraRotationUpdate_Implementation(float NewPitch) {
	Pitch = NewPitch;
	OnRep_Pitch();
}

void AMyCharacter::OnRep_Pitch() {
	if (CameraComponent) {
		FRotator NewRotation = FRotator(Pitch, CameraComponent->GetComponentRotation().Yaw, CameraComponent->GetComponentRotation().Roll);
		CameraComponent->SetWorldRotation(NewRotation);
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
	if (!GetCharacterMovement()->IsCrouching() && GetCharacterMovement()->Velocity.Size() > 0)
	{
		bIsSprinting = true;
		OnRep_SprintState();
	}
}

void AMyCharacter::StopSprint()
{
	bIsSprinting = false;
	OnRep_SprintState();
}

void AMyCharacter::ServerStartSprint_Implementation()
{
	StartSprint();
}

void AMyCharacter::ServerStopSprint_Implementation()
{
	StopSprint();
}

void AMyCharacter::OnRep_SprintState()
{
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyCharacter, bIsSprinting);
	DOREPLIFETIME(AMyCharacter, HitActor);
	DOREPLIFETIME(AMyCharacter, Pitch);
}

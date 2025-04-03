#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class ECHOESOFTIME_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

	// Movement and looking functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void StartCrouch();
	void StopCrouching();

	// Sprint functions
	void StartSprint();
	void StopSprint();

	// Map switching mechanics
	void MapSwitch(const FInputActionValue& Value);
	void MapSwitchReleased(const FInputActionValue& Value);

	// Input actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SwitchMapAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PickupAction;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	class UTimeManager* TimeManager;

	void Pickup();
	void Drop();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ActorDistance=150.f;


	// Map switching state
	bool bCanSwitchMap = true;

	// Server-side map switching
	UFUNCTION(Server, Reliable)
	void ServerMapSwitch();

	// Server-side sprinting
	UFUNCTION(Server, Reliable)
	void ServerStartSprint();
	UFUNCTION(Server, Reliable)
	void ServerStopSprint();


	// Replicated sprint state
	UPROPERTY(ReplicatedUsing = OnRep_SprintState)
	bool bIsSprinting;

	UFUNCTION()
	void OnRep_SprintState();




public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

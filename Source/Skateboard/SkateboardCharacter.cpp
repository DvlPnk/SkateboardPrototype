// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateboardCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/S_CustomMovementComponent.h"


//////////////////////////////////////////////////////////////////////////
// ASkateboardCharacter

ASkateboardCharacter::ASkateboardCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<US_CustomMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Setting init values to movement properties
	bAbleToImpulse = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CustomMovementComponent = Cast<US_CustomMovementComponent>(GetCharacterMovement()); // SetCustomMovementComponent

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character does not moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Creating and attaching Skateboard mesh to center of mass socket
	SkateMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Skateboard Mesh")); 
	SkateMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("center_of_massSocket")); 
}

void ASkateboardCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	
}

void ASkateboardCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UE_LOG(LogTemp, Warning, TEXT("%f"), CustomMovementComponent->MaxWalkSpeed);

	const FRotator Rotation = GetActorRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	if (bIsTryingToImpulse)
	{
		AddMovementInput(ForwardDirection, 1);
	}

	if(bIsDecelerating)
	{
		
	}
}

// Input

void ASkateboardCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateboardCharacter::Move);
		EnhancedInputComponent->BindAction(ReleaseForwardAction, ETriggerEvent::Completed, this, &ASkateboardCharacter::StopForwardMovement);
		EnhancedInputComponent->BindAction(ReleaseHorizontalAction, ETriggerEvent::Completed, this, &ASkateboardCharacter::StopHorizontalMovement);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASkateboardCharacter::Look);
	}
}

void ASkateboardCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if(MovementVector.Y > 0 && CustomMovementComponent->MovementMode != MOVE_Falling)
		{
			bIsTryingToImpulse = true;
			bIsDecelerating = false;
			CurrentDecelerationSpeed = CustomMovementComponent->ImpulseDecelerationSpeed;
			bIsMovingHorizontally = false;
			if(bAbleToImpulse)
			{
				bIsImpulsing = true;
				SpeedUpSkateboard();
				FTimerHandle TimerHandle;
				bAbleToImpulse = false;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]
					{
						bAbleToImpulse = true;
					}, 1, false);
			}
		}
		else if(MovementVector.Y < 0)
		{
			bIsTryingToImpulse = false;
			bIsDecelerating = true;

			CurrentDecelerationSpeed = CustomMovementComponent->DecelerationSpeed;
			CustomMovementComponent->MaxWalkSpeed = FMath::Clamp(GetCharacterMovement()->MaxWalkSpeed - CurrentDecelerationSpeed,
				0, CustomMovementComponent->MaxSkateSpeed);
			if (YawRotation.UnrotateVector(GetCharacterMovement()->Velocity).X < 100)
			{
				GetCharacterMovement()->MaxWalkSpeed = 0;
			}
			else
			{
				AddMovementInput(ForwardDirection, 1);
			}
		}

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if(YawRotation.UnrotateVector(GetCharacterMovement()->Velocity).X > 50)
		{
			float ControlMove = MovementVector.X * CustomMovementComponent->MaxWalkSpeed *
				(CustomMovementComponent->MovementMode == MOVE_Walking ? CustomMovementComponent->FloorRotationControl 
					: CustomMovementComponent->AirRotationControl);

			float FinalMovementXValue = NormalizeValue(ControlMove, -CustomMovementComponent->MaxSkateSpeed,
				CustomMovementComponent->MaxSkateSpeed, 2, true);

			bIsMovingHorizontally = FMath::Abs(FinalMovementXValue) >= 0.1;

			AddActorWorldRotation(FRotator(0, FinalMovementXValue, 0));
		}
	}
}

void ASkateboardCharacter::StopForwardMovement(const FInputActionValue& Value)
{
	bIsImpulsing = false;
}

void ASkateboardCharacter::StopHorizontalMovement(const FInputActionValue& Value)
{
	bIsMovingHorizontally = false;
}

void ASkateboardCharacter::Look(const FInputActionValue& Value)
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

float ASkateboardCharacter::NormalizeValue(float Source, float MinVal, float MaxVal, float Multiplier,
	bool bAffectsMultiplier)
{
	float PrevResult = (Source - MinVal) / (MaxVal - MinVal);
	if (bAffectsMultiplier)
	{
		return Multiplier * PrevResult - Multiplier / 2;
	}
	return PrevResult;
}

void ASkateboardCharacter::SpeedUpSkateboard()
{
	GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(GetCharacterMovement()->MaxWalkSpeed +
		CustomMovementComponent->ImpulseSpeed, 0, CustomMovementComponent->MaxSkateSpeed);
}

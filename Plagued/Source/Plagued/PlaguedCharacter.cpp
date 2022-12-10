// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlaguedCharacter.h"
#include "PlaguedProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TP_WeaponComponent.h"


//////////////////////////////////////////////////////////////////////////
// APlaguedCharacter

APlaguedCharacter::APlaguedCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void APlaguedCharacter::BeginPlay()
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

void APlaguedCharacter::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (M_IsZooming)
	{
		GetFirstPersonCameraComponent()->SetFieldOfView(FMath::Lerp(GetFirstPersonCameraComponent()->FieldOfView, 40.0f, DeltaSeconds * 10.0f));
	}
	else
	{
		GetFirstPersonCameraComponent()->SetFieldOfView(FMath::Lerp(GetFirstPersonCameraComponent()->FieldOfView, 80.0f, DeltaSeconds * 10.0f));
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void APlaguedCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::Look);

		EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::Test);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::FireWeapon);

		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::Zoom);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &APlaguedCharacter::StopZoom);
	}
}


void APlaguedCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APlaguedCharacter::Look(const FInputActionValue& Value)
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

void APlaguedCharacter::Zoom()
{
	M_IsZooming = true;
}

void APlaguedCharacter::StopZoom()
{
	M_IsZooming = false;
}

bool APlaguedCharacter::Server_Test_Validate()
{
	return true;
}

void APlaguedCharacter::Server_Test_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("TestRPC"));
}

void APlaguedCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

void APlaguedCharacter::SetRifle(UTP_WeaponComponent* _rifle)
{
	M_CurrentWeapon = _rifle;

	if (_rifle != nullptr)
	{
		bHasRifle = true;
	}
	else
	{
		bHasRifle = false;
	}
}

bool APlaguedCharacter::GetHasRifle()
{
	return bHasRifle;
}

void APlaguedCharacter::Test()
{
	Server_Test();
}

void APlaguedCharacter::FireWeapon()
{
	if (!HasAuthority())
	{
		Server_FireWeapon();
	}
	else if (M_CurrentWeapon != nullptr)
	{
		M_CurrentWeapon->Fire();
	}
}

bool APlaguedCharacter::Server_FireWeapon_Validate()
{
	return true;
}

void APlaguedCharacter::Server_FireWeapon_Implementation()
{
	if (M_CurrentWeapon != nullptr)
	{
		M_CurrentWeapon->Fire();
	}
}

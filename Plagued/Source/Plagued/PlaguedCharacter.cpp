// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlaguedCharacter.h"
#include "PlaguedProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "CW_HUD.h"
#include "CW_Inventory.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Pickup_Base.h"
#include "Door_Base.h"
#include "TP_WeaponComponent.h"
#include "GameFramework/GameSession.h"


//////////////////////////////////////////////////////////////////////////
// APlaguedCharacter

APlaguedCharacter::APlaguedCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create Arm
	Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Arm->TargetArmLength = 300.0f;
	Arm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

	// Init Arm Lag
	//Arm->bEnableCameraLag = true;
	//Arm->CameraLagSpeed = 2.0f;
	//Arm->CameraLagMaxDistance = 1.5f;
	//Arm->bEnableCameraRotationLag =true;
	//Arm->CameraRotationLagSpeed = 4.0f;
	//Arm->CameraLagMaxTimeStep = 1.0f;
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

	ToggleHUD();
}

void APlaguedCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerHUD)
	{
		PlayerHUD->RemoveFromParent();
		PlayerHUD = nullptr;
	}
	if (PlayerInventory)
	{
		PlayerInventory->RemoveFromParent();
		PlayerInventory = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

void APlaguedCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	Arm->AttachToComponent(RootComponent, AttachmentRules);
	
	AttachCameraToFPP();
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

	InteractRaycast();
}

void APlaguedCharacter::AttachCameraToTPP()
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	FirstPersonCameraComponent->AttachToComponent(Arm, AttachmentRules, USpringArmComponent::SocketName);
}

void APlaguedCharacter::AttachCameraToFPP()
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	FirstPersonCameraComponent->AttachToComponent(GetMesh(), AttachmentRules, FName("head"));
}

void APlaguedCharacter::ChangePerspective()
{
	M_IsFirstPerson = !M_IsFirstPerson;

	if (M_IsFirstPerson)
	{
		AttachCameraToFPP();
	}
	else
	{
		AttachCameraToTPP();
	}
}

void APlaguedCharacter::InteractRaycast()
{
	FHitResult* result = new FHitResult;
	FVector startTrace = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector traceDir = GetFirstPersonCameraComponent()->GetForwardVector();
	FVector endTrace = traceDir * 500.0f + startTrace;
	FCollisionQueryParams* queryParams = new FCollisionQueryParams;

	if (PlayerHUD)
		PlayerHUD->ShowInteractText(false);
	
	if (GetWorld()->LineTraceSingleByChannel(*result, startTrace, endTrace, ECC_Visibility, *queryParams))
	{
		if (APickup_Base* pickup = Cast<APickup_Base>(result->GetActor()))
		{
			if (PlayerHUD)
				PlayerHUD->ShowInteractText(true);
		}
		if (ADoor_Base* door = Cast<ADoor_Base>(result->GetActor()))
		{
			if (PlayerHUD)
				PlayerHUD->ShowInteractText(true);
		}
	}

	LastRaycastHit = result;
}

void APlaguedCharacter::TryInteract()
{
	if (LastRaycastHit)
	{
		if (APickup_Base* pickup = Cast<APickup_Base>(LastRaycastHit->GetActor()))
		{
			PickupItem(pickup);
		}
		if (ADoor_Base* door = Cast<ADoor_Base>(LastRaycastHit->GetActor()))
		{
			OpenDoor(door);
		}
	}
}

void APlaguedCharacter::ToggleInventoryMenu()
{
	if (IsLocallyControlled() && PlayerInventoryClass)
	{
		if (PlayerInventory)
		{
			PlayerInventory->RemoveFromParent();
			PlayerInventory = nullptr;

			ToggleHUD();
		}
		else
		{
			APlayerController* controller = GetController<APlayerController>();
			check(controller);
			PlayerInventory = CreateWidget<UCW_Inventory>(controller, PlayerInventoryClass);
			check(PlayerInventory);
			PlayerInventory->AddToPlayerScreen();

			ToggleHUD();
		}
	}
}

void APlaguedCharacter::ToggleHUD()
{
	if (IsLocallyControlled() && PlayerHUDClass)
	{
		if (PlayerHUD)
		{
			PlayerHUD->RemoveFromParent();
			PlayerHUD = nullptr;
		}
		else
		{
			APlayerController* controller = GetController<APlayerController>();
			check(controller);
			PlayerHUD = CreateWidget<UCW_HUD>(controller, PlayerHUDClass);
			check(PlayerHUD);
			PlayerHUD->AddToPlayerScreen();
		}
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

		EnhancedInputComponent->BindAction(ChangePerspectiveAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::ChangePerspective);

		EnhancedInputComponent->BindAction(Interact, ETriggerEvent::Triggered, this, &APlaguedCharacter::TryInteract);

		EnhancedInputComponent->BindAction(ToggleInventory, ETriggerEvent::Triggered, this, &APlaguedCharacter::ToggleInventoryMenu);
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

void APlaguedCharacter::PickupItem(APickup_Base* _pickup)
{
	GetHUD()->SetSlot(_pickup->Icon, 0);
	
	if (!HasAuthority())
	{
		Server_PickupItem(_pickup);
	}
	else
	{
		_pickup->Pickup(this);
	}
}

void APlaguedCharacter::OpenDoor(ADoor_Base* _door)
{
	if (!HasAuthority())
	{
		Server_OpenDoor(_door);
	}
	else
	{
		_door->Interact(GetActorLocation());
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

bool APlaguedCharacter::Server_PickupItem_Validate(APickup_Base* _pickup)
{
	return true;
}

void APlaguedCharacter::Server_PickupItem_Implementation(APickup_Base* _pickup)
{
	_pickup->Pickup(this);
}

bool APlaguedCharacter::Server_OpenDoor_Validate(ADoor_Base* _door)
{
	return true;
}

void APlaguedCharacter::Server_OpenDoor_Implementation(ADoor_Base* _door)
{
	_door->Interact(GetActorLocation());
}

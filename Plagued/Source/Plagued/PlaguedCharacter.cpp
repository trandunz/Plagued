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
#include "Weapon_Melee.h"
#include "Door_Base.h"
#include "TP_WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameSession.h"
#include "IInteractInterface.h"
#include "AC_InventorySystem.h"
#include "AM1911.h"
#include "CIKAnimInstance.h"
#include "ItemData.h"
#include "CGunComponent.h"
#include "Net/UnrealNetwork.h"


//////////////////////////////////////////////////////////////////////////
// APlaguedCharacter

APlaguedCharacter::APlaguedCharacter()
{
	//bReplicates = true;
	//bAlwaysRelevant = true;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	AttachCameraToFPP();

	// Create Arm
	Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Arm->TargetArmLength = 150.0f;
	Arm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	Arm->AttachToComponent(GetMesh(), AttachmentRules, FName("Head"));
	Arm->SocketOffset = {0,70,-20};

	GetCharacterMovement()->SetIsReplicated(true);

	InventorySystem = CreateDefaultSubobject<UAC_InventorySystem>(TEXT("Inventory_System"));

	// Init Arm Lag
	//Arm->bEnableCameraLag = true;
	//Arm->CameraLagSpeed = 2.0f;
	//Arm->CameraLagMaxDistance = 1.5f;
	//Arm->bEnableCameraRotationLag =true;
	//Arm->CameraRotationLagSpeed = 4.0f;
	//Arm->CameraLagMaxTimeStep = 1.0f;
}

void APlaguedCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlaguedCharacter, IsMeleeStance);
	DOREPLIFETIME(APlaguedCharacter, TryMeleeAttack);
	DOREPLIFETIME(APlaguedCharacter, CanMeleeAttack);
	DOREPLIFETIME(APlaguedCharacter, MovementSpeed);
	DOREPLIFETIME(APlaguedCharacter, RifleEquiped);
	DOREPLIFETIME(APlaguedCharacter, IsAiming);
	DOREPLIFETIME(APlaguedCharacter, EquipedItem);
	DOREPLIFETIME(APlaguedCharacter, SpineRotationX);
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

	AnimationInstance = Cast<UCIKAnimInstance>(GetMesh()->GetAnimInstance());
	
	PlayerHUD->ShowAmmoText(false);
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

	//AimTimeline.TickTimeline(DeltaSeconds);

	if (IsZooming)
	{
		GetFirstPersonCameraComponent()->SetFieldOfView(FMath::Lerp(GetFirstPersonCameraComponent()->FieldOfView, 40.0f, DeltaSeconds * 10.0f));
	}
	else
	{
		GetFirstPersonCameraComponent()->SetFieldOfView(FMath::Lerp(GetFirstPersonCameraComponent()->FieldOfView, 80.0f, DeltaSeconds * 10.0f));
	}

	InteractRaycast();

	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(GetCharacterMovement()->MaxWalkSpeed, MovementSpeed, DeltaSeconds * 10.0f);

	if (EquipedItem)
	{
		if (IIGunInterface* gun = Cast<IIGunInterface>(EquipedItem))
		{
			if (PlayerHUD)
			{
				PlayerHUD->ShowAmmoText(gun->CurrentAmmo, gun->MaxAmmo, 0);
				PlayerHUD->ShowFireType(true, gun->GetGunComponent()->FireType);
			}
		}
	}
	else if (PlayerHUD)
	{
		PlayerHUD->ShowAmmoText(false);
		PlayerHUD->ShowFireType(false);
	}

	if (FireTimer > 0)
	{
		FireTimer -= DeltaSeconds;
	}
	else
	{
		FireTimer = FireRate;

		if (IsFiring)
		{
			FireWeapon();
		}
	}
 }

void APlaguedCharacter::AttachCameraToTPP()
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	FirstPersonCameraComponent->AttachToComponent(Arm, AttachmentRules, USpringArmComponent::SocketName);
}

void APlaguedCharacter::AttachCameraToFPP()
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	FirstPersonCameraComponent->AttachToComponent(GetMesh(), AttachmentRules, FName("Head"));
}

void APlaguedCharacter::ChangePerspective()
{
	IsFirstPerson = !IsFirstPerson;

	if (IsFirstPerson)
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
		if (IIInteractInterface* Pickup = Cast<IIInteractInterface>(result->GetActor()))
		{
			if (PlayerHUD)
				PlayerHUD->ShowInteractText(true, Pickup->LookAt());
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
		if (IIInteractInterface* Pickup = Cast<IIInteractInterface>(LastRaycastHit->GetActor()))
		{
			TryInteractWithInterface(LastRaycastHit->GetActor());
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

			APlayerController* controller = GetController<APlayerController>();
			controller->SetShowMouseCursor(false);
			controller->SetInputMode(FInputModeGameOnly());
			
			ToggleHUD();
		}
		else
		{
			APlayerController* controller = GetController<APlayerController>();
			check(controller);
			PlayerInventory = CreateWidget<UCW_Inventory>(controller, PlayerInventoryClass);
			check(PlayerInventory);
			PlayerInventory->AddToPlayerScreen();
			controller->SetInputMode(FInputModeGameAndUI());
			controller->SetShowMouseCursor(true);

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

void APlaguedCharacter::StartAim()
{
	//IsMeleeStance = true;
	

	if (!HasAuthority())
	{
		Server_StartAim();
	}
	else
	{
		IsAiming = true;
	}

	//AimTimeline.Play();
}

void APlaguedCharacter::EndAim()
{
	//IsMeleeStance = false;
	if (!HasAuthority())
	{
		Server_StopAim();
	}
	else
	{
		IsAiming = false;
	}
	

	//AimTimeline.Reverse();
}

void APlaguedCharacter::TryMelee()
{
	Multi_TryMelee();
}

void APlaguedCharacter::StartSprint()
{
	if (!HasAuthority())
	{
		Server_StartSprint();
	}
	else
	{
		MovementSpeed = SprintSpeed;
	}
}

void APlaguedCharacter::EndSprint()
{
	if (!HasAuthority())
	{
		Server_EndSprint();
	}
	else
	{
		MovementSpeed = WalkSpeed;
	}
}

void APlaguedCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::Look);

		EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::Test);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &APlaguedCharacter::EndFire);

		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::Zoom);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &APlaguedCharacter::StopZoom);

		EnhancedInputComponent->BindAction(ChangePerspectiveAction, ETriggerEvent::Triggered, this, &APlaguedCharacter::ChangePerspective);

		EnhancedInputComponent->BindAction(Interact, ETriggerEvent::Triggered, this, &APlaguedCharacter::TryInteract);

		EnhancedInputComponent->BindAction(ToggleInventory, ETriggerEvent::Triggered, this, &APlaguedCharacter::ToggleInventoryMenu);

		EnhancedInputComponent->BindAction(Aim, ETriggerEvent::Triggered, this, &APlaguedCharacter::StartAim);
		EnhancedInputComponent->BindAction(Aim, ETriggerEvent::Completed, this, &APlaguedCharacter::EndAim);

		EnhancedInputComponent->BindAction(Sprint, ETriggerEvent::Triggered, this, &APlaguedCharacter::StartSprint);
		EnhancedInputComponent->BindAction(Sprint, ETriggerEvent::Completed, this, &APlaguedCharacter::EndSprint);

		EnhancedInputComponent->BindAction(Reload, ETriggerEvent::Triggered, this, &APlaguedCharacter::TryReloadGun);

		EnhancedInputComponent->BindAction(ChangeFirerate, ETriggerEvent::Triggered, this, &APlaguedCharacter::ChangeFireRate);
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

		if (!HasAuthority())
		{
			Server_SpineRotation(GetControlRotation().Pitch * -1);
		}
		else
		{
			SpineRotationX = {0, 0, GetControlRotation().Pitch * -1};
		}
	}
}

void APlaguedCharacter::Zoom()
{
	IsZooming = true;
}

void APlaguedCharacter::StopZoom()
{
	IsZooming = false;
}

void APlaguedCharacter::TryReloadGun()
{
	if (!HasAuthority())
	{
		Server_TryReload();
	}
	else
	{
		if (EquipedItem)
		{
			if (IIGunInterface* gun = Cast<IIGunInterface>(EquipedItem))
			{
				UE_LOG(LogTemp, Warning, TEXT("Reload Gun"));
				gun->Reload();
			}
		}
	}
}

void APlaguedCharacter::StartFire()
{
	if (EquipedItem != nullptr)
	{
		if (IIGunInterface* gun = Cast<IIGunInterface>(EquipedItem))
		{
			IsFiring = true;
		}
	}
}

void APlaguedCharacter::EndFire()
{
	IsFiring = false;

	if (EquipedItem != nullptr)
	{
		if (IIGunInterface* gun = Cast<IIGunInterface>(EquipedItem))
		{
			gun->ReleaseMouse();
		}
	}
}

void APlaguedCharacter::ChangeFireRate()
{
	if (EquipedItem != nullptr)
	{
		if (IIGunInterface* gun = Cast<IIGunInterface>(EquipedItem))
		{
			gun->ChangeFireType();
		}
	}
	
}

bool APlaguedCharacter::Server_Test_Validate()
{
	return true;
}

void APlaguedCharacter::Server_Test_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("TestRPC"));
}

bool APlaguedCharacter::Server_SpineRotation_Validate(float _xRotation)
{
	return true;
}

void APlaguedCharacter::Server_SpineRotation_Implementation(float _xRotation)
{
	SpineRotationX = {0, 0, _xRotation};
}

bool APlaguedCharacter::Multi_SpineRotation_Validate(float _xRotation)
{
	return true;
}

void APlaguedCharacter::Multi_SpineRotation_Implementation(float _xRotation)
{
	SpineRotationX = {0, 0, _xRotation};
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
	else
	{
		if (EquipedItem != nullptr)
		{
			Cast<IIGunInterface>(EquipedItem)->Fire();
		}
	}
}

void APlaguedCharacter::TryInteractWithInterface(AActor* _pickup)
{
	if (!HasAuthority())
	{
		Server_Interact(_pickup);
	}
	else
	{
		if (UItemData* itemdata = _pickup->FindComponentByClass<UItemData>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Item Added"));
			InventorySystem->AddToInventory(itemdata);
		}
		Cast<IIInteractInterface>(_pickup)->Interact();
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

void APlaguedCharacter::ToggleWeapon(AActor* _weapon)
{
	if (AWeapon_Melee* meleeWeapon = Cast<AWeapon_Melee>(_weapon))
	{
	}
	else if (IIGunInterface* gun = Cast<IIGunInterface>(_weapon))
	{
		RifleEquiped = true;
		
		gun->InstigatorController = Cast<APlayerController>(GetController());
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		_weapon->AttachToComponent(GetMesh(), AttachmentRules, FName("S_HandR"));
	}
}

void APlaguedCharacter::EquipItem(TSubclassOf<AActor> _class)
{
	UE_LOG(LogTemp, Warning, TEXT("Try Equip Item"));
	
	if (!HasAuthority())
	{
		Server_EquipItem(_class);
	}
	else
	{
		if (EquipedItem)
		{
			EquipedItem->GetOwner()->Destroy();
		}
	
		EquipedItem = GetWorld()->SpawnActor<AActor>(_class);
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		EquipedItem->AttachToComponent(RootComponent, AttachmentRules);

		ToggleWeapon(EquipedItem);
	}
}

bool APlaguedCharacter::Server_FireWeapon_Validate()
{
	return true;
}

void APlaguedCharacter::Server_FireWeapon_Implementation()
{
	if (EquipedItem != nullptr)
	{
		if (IIGunInterface* gun = Cast<IIGunInterface>(EquipedItem))
		{
			gun->Fire();
		}
	}
}

bool APlaguedCharacter::Server_Interact_Validate(AActor* _pickup)
{
	return true;
}

void APlaguedCharacter::Server_Interact_Implementation(AActor* _pickup)
{
	if (UItemData* itemdata = _pickup->FindComponentByClass<UItemData>())
	{
		InventorySystem->AddToInventory(itemdata);
	}
	Cast<IIInteractInterface>(_pickup)->Interact();
}

bool APlaguedCharacter::Server_OpenDoor_Validate(ADoor_Base* _door)
{
	return true;
}

void APlaguedCharacter::Server_OpenDoor_Implementation(ADoor_Base* _door)
{
	_door->Interact(GetActorLocation());
}

bool APlaguedCharacter::Multi_TryMelee_Validate()
{
	return true;
}

void APlaguedCharacter::Multi_TryMelee_Implementation()
{
	if (IsMeleeStance && CanMeleeAttack)
	{
		CanMeleeAttack = false;
		TryMeleeAttack = true;
	}
}

bool APlaguedCharacter::Server_TryMelee_Validate()
{
	return true;
}

void APlaguedCharacter::Server_TryMelee_Implementation()
{
	if (IsMeleeStance && CanMeleeAttack)
	{
		CanMeleeAttack = false;
		TryMeleeAttack = true;
	}
}

bool APlaguedCharacter::Server_EquipItem_Validate(TSubclassOf<AActor> _class)
{
	return true;
}

void APlaguedCharacter::Server_EquipItem_Implementation(TSubclassOf<AActor> _class)
{
	if (EquipedItem)
	{
		EquipedItem->GetOwner()->Destroy();
	}
	
	EquipedItem = GetWorld()->SpawnActor<AActor>(_class);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	EquipedItem->AttachToComponent(RootComponent, AttachmentRules);
	
	ToggleWeapon(EquipedItem);
}

bool APlaguedCharacter::Server_StartSprint_Validate()
{
	return true;
}

void APlaguedCharacter::Server_StartSprint_Implementation()
{
	MovementSpeed = SprintSpeed;
}

bool APlaguedCharacter::Server_EndSprint_Validate()
{
	return true;
}

void APlaguedCharacter::Server_EndSprint_Implementation()
{
	MovementSpeed = WalkSpeed;
}

bool APlaguedCharacter::Server_StartAim_Validate()
{
	return true;
}

void APlaguedCharacter::Server_StartAim_Implementation()
{
	IsAiming = true;
}

bool APlaguedCharacter::Server_StopAim_Validate()
{
	return true;
}

void APlaguedCharacter::Server_StopAim_Implementation()
{
	IsAiming = false;
}

bool APlaguedCharacter::Server_TryReload_Validate()
{
	return true;
}

void APlaguedCharacter::Server_TryReload_Implementation()
{
	if (EquipedItem)
	{
		if (IIGunInterface* gun = Cast<IIGunInterface>(EquipedItem))
		{
			gun->Reload();
		}
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IGunInterface.h"
#include "IInteractInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlaguedCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class APlaguedCharacter : public ACharacter
{
	GENERATED_BODY()
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* TestAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ChangePerspectiveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* Interact;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ToggleInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* Aim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* Sprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* Reload;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ChangeFirerate;

public:
	APlaguedCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly);
	bool IsMeleeStance = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly);
	bool TryMeleeAttack = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite);
	bool CanMeleeAttack = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly);
	bool IsAiming = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly);
	bool RifleEquiped = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite);
	float MovementSpeed = 500.0f;

	UPROPERTY()
	bool IsFiring = false;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCW_HUD> PlayerHUDClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCW_Inventory> PlayerInventoryClass;
	
	UPROPERTY()
	class UCW_Inventory* PlayerInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess = "true"))
	class UAC_InventorySystem* InventorySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	float SprintSpeed = 900.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	float WalkSpeed = 500.0f;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly);
	AActor* EquipedItem;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	FRotator SpineRotationX;

	UPROPERTY(EditAnywhere)
	float FireRate = 0.1;
protected:
	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	void AttachCameraToTPP();
	void AttachCameraToFPP();
	void ChangePerspective();
	
	void InteractRaycast();
	void TryInteract();

	void ToggleInventoryMenu();
	void ToggleHUD();

	void StartAim();
	void EndAim();

	void TryMelee();

	void StartSprint();
	void EndSprint();

	void Zoom();
	void StopZoom();

	void TryReloadGun();

	void StartFire();
	void EndFire();

	void ChangeFireRate();

	FHitResult* LastRaycastHit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=MyCharacter, meta=(AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY()
	USpringArmComponent* Arm;
	
	UPROPERTY()
	class UCW_HUD* PlayerHUD;
	
	class UCIKAnimInstance* AnimationInstance;

	bool IsZooming = false;
	float ZoomRatio = 0.0f;
	bool IsFirstPerson = true;

	float FireTimer = 0.0f;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UFUNCTION(BlueprintCallable)
	void Test();

	UFUNCTION(BlueprintCallable)
	void FireWeapon();

	UFUNCTION(BlueprintCallable)
	void TryInteractWithInterface(AActor* _pickup);

	UFUNCTION(BlueprintCallable)
	void OpenDoor(ADoor_Base* _door);
	
	UFUNCTION(BlueprintCallable)
	void ToggleWeapon(AActor* _weapon = nullptr);

	UFUNCTION(BlueprintCallable)
	void EquipItem(TSubclassOf<AActor> _class);

	/////////////////////////////////////////////////////////////////////////////////////
protected:
	//
	// REPLICATION FUNCTIONS
	//
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Test();
	bool Server_Test_Validate();
	void Server_Test_Implementation();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SpineRotation(float _xRotation);
	bool Server_SpineRotation_Validate(float _xRotation);
	void Server_SpineRotation_Implementation(float _xRotation);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_SpineRotation(float _xRotation);
	bool Multi_SpineRotation_Validate(float _xRotation);
	void Multi_SpineRotation_Implementation(float _xRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_FireWeapon();
	bool Server_FireWeapon_Validate();
	void Server_FireWeapon_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact(AActor* _pickup);
	bool Server_Interact_Validate(AActor* _pickup);
	void Server_Interact_Implementation(AActor* _pickup);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OpenDoor(ADoor_Base* _door);
	bool Server_OpenDoor_Validate(ADoor_Base* _door);
	void Server_OpenDoor_Implementation(ADoor_Base* _door);
	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_TryMelee();
	bool Multi_TryMelee_Validate();
	void Multi_TryMelee_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TryMelee();
	bool Server_TryMelee_Validate();
	void Server_TryMelee_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipItem(TSubclassOf<AActor> _class);
	bool Server_EquipItem_Validate(TSubclassOf<AActor> _class);
	void Server_EquipItem_Implementation(TSubclassOf<AActor> _class);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartSprint();
	bool Server_StartSprint_Validate();
	void Server_StartSprint_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EndSprint();
	bool Server_EndSprint_Validate();
	void Server_EndSprint_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartAim();
	bool Server_StartAim_Validate();
	void Server_StartAim_Implementation();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopAim();
	bool Server_StopAim_Validate();
	void Server_StopAim_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TryReload();
	bool Server_TryReload_Validate();
	void Server_TryReload_Implementation();

public:
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	UCW_HUD* GetHUD() const {return PlayerHUD;}
};

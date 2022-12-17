// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IGunInterface.h"
#include "IInteractInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
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
	
	/** First person camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=MyCharacter, meta=(AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=MyCharacter, meta=(AllowPrivateAccess = "true"))
	USceneComponent* MeleeWeaponSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=MyCharacter, meta=(AllowPrivateAccess = "true"))
	USceneComponent* HandGunSocket;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon_Melee> MeleeAsset;
	
	UPROPERTY()
	USpringArmComponent* Arm;
	
	UPROPERTY()
	class UCW_HUD* PlayerHUD;
	
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

public:
	APlaguedCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void Jump();
	void StopJumping();
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly);
	bool IsMeleeStance = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly);
	bool TryMeleeAttack = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite);
	bool CanMeleeAttack = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly);
	bool IsAiming = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite);
	float MovementSpeed = 500.0f;
	
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

protected:
	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;

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

	FHitResult* LastRaycastHit = nullptr;
public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);
	
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetRifle(UTP_WeaponComponent* _rifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

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
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Zoom();
	void StopZoom();

	bool M_IsZooming = false;
	float M_ZoomRatio = 0.0f;
	bool M_IsFirstPerson = true;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Test();
	bool Server_Test_Validate();
	void Server_Test_Implementation();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	UTP_WeaponComponent* M_CurrentWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	AWeapon_Melee* M_MeleeWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	AActor* EquipedWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	AActor* M_EquipedItem = nullptr;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UCW_HUD* GetHUD() const {return PlayerHUD;}

};

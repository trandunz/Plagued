// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "CCZombie.generated.h"

UCLASS()
class PLAGUED_API ACCZombie : public ACharacter
{
	GENERATED_BODY()

public:
	ACCZombie();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION()
	void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);

	UFUNCTION()
	void OnSeePawn(APawn *OtherPawn);

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float _amount);

	void TakeDamage(float _amount, FVector _bulletDirection, FVector _hitpoint, FName _boneName);
	
	UFUNCTION()
	void OnDeath();

	void OnDeath(FVector _bulletDirection, FVector _hitpoint, FName _boneName);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	bool IsRagdoll = false;

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Attack();
	bool Server_Attack_Validate();
	void Server_Attack_Implementation();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Attack();
	bool Multi_Attack_Validate();
	void Multi_Attack_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TakeDamage(float _amount);
	bool Server_TakeDamage_Validate(float _amount);
	void Server_TakeDamage_Implementation(float _amount);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TakeDamageWithImpulse(float _amount, FVector _bulletDirection, FVector _hitpoint, FName _boneName);
	bool Server_TakeDamageWithImpulse_Validate(float _amount, FVector _bulletDirection, FVector _hitpoint, FName _boneName);
	void Server_TakeDamageWithImpulse_Implementation(float _amount, FVector _bulletDirection, FVector _hitpoint, FName _boneName);
};

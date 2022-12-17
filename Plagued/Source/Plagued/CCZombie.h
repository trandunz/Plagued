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
	// Sets default values for this character's properties
	ACCZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Attack();
	bool Server_Attack_Validate();
	void Server_Attack_Implementation();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Attack();
	bool Multi_Attack_Validate();
	void Multi_Attack_Implementation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	void Attack();
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	UFUNCTION()
	void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);

	UFUNCTION()
	void OnSeePawn(APawn *OtherPawn);
};

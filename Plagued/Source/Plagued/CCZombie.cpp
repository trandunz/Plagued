// Fill out your copyright notice in the Description page of Project Settings.


#include "CCZombie.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameSession.h"

// Sets default values
ACCZombie::ACCZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
	GetCharacterMovement()->MaxWalkSpeed = 60.0f;
}

// Called when the game starts or when spawned
void ACCZombie::BeginPlay()
{
	Super::BeginPlay();
}

bool ACCZombie::Server_Attack_Validate()
{
	return true;
}

void ACCZombie::Server_Attack_Implementation()
{
	Multi_Attack();
}

bool ACCZombie::Multi_Attack_Validate()
{
	return true;
}

void ACCZombie::Multi_Attack_Implementation()
{
	PlayAnimMontage(AttackMontage);
}

// Called every frame
void ACCZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACCZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACCZombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ACCZombie::OnSeePawn);
		PawnSensing->OnHearNoise.AddDynamic(this, &ACCZombie::OnHearNoise);
	}
}

void ACCZombie::Attack()
{
	Server_Attack();
}

void ACCZombie::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
}

void ACCZombie::OnSeePawn(APawn* OtherPawn)
{
	GetCharacterMovement()->MaxWalkSpeed = 360;
}


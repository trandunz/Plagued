// Fill out your copyright notice in the Description page of Project Settings.


#include "CCZombie.h"

#include "IBulletInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameSession.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

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
	
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
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

bool ACCZombie::Server_TakeDamage_Validate(float _amount)
{
	return true;
}

void ACCZombie::Server_TakeDamage_Implementation(float _amount)
{
	CurrentHealth -= _amount;
	if (CurrentHealth <= 0)
	{
		OnDeath();
	}
}

bool ACCZombie::Server_TakeDamageWithImpulse_Validate(float _amount, FVector _bulletDirection, FVector _hitpoint, FName _boneName)
{
	return true;
}

void ACCZombie::Server_TakeDamageWithImpulse_Implementation(float _amount, FVector _bulletDirection, FVector _hitpoint, FName _boneName)
{
	CurrentHealth -= _amount;
	if (CurrentHealth <= 0)
	{
		OnDeath(_bulletDirection, _hitpoint, _boneName);
	}
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

void ACCZombie::TakeDamage(float _amount)
{
	if (CurrentHealth > 0)
	{
		if (!HasAuthority())
		{
			Server_TakeDamage(_amount);
		}
		else
		{
			CurrentHealth -= _amount;
			if (CurrentHealth <= 0)
			{
				OnDeath();
			}
		}
	}
	
}

void ACCZombie::TakeDamage(float _amount, FVector _bulletDirection, FVector _hitpoint, FName _boneName)
{
	if (CurrentHealth > 0)
	{
		if (!HasAuthority())
		{
			Server_TakeDamageWithImpulse(_amount, _bulletDirection, _hitpoint, _boneName);
		}
		else
		{
			CurrentHealth -= _amount;
			if (CurrentHealth <= 0)
			{
				OnDeath(_bulletDirection, _hitpoint, _boneName);
			}
		}
	}
}

void ACCZombie::OnDeath()
{
	SetReplicateMovement(false);

	DetachFromControllerPendingDestroy();
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	SetActorEnableCollision(true);

	if (!IsRagdoll)
	{
		// Ragdoll
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;

		UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
		if (CharacterComp)
		{
			CharacterComp->StopMovementImmediately();
			CharacterComp->DisableMovement();
			CharacterComp->SetComponentTickEnabled(false);
		}

		SetLifeSpan(10.0f);
		IsRagdoll = true;
	}
}

void ACCZombie::OnDeath(FVector _bulletDirection, FVector _hitpoint, FName _boneName)
{
	SetReplicateMovement(false);

	DetachFromControllerPendingDestroy();
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	SetActorEnableCollision(true);

	if (!IsRagdoll)
	{
		// Ragdoll
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;

		UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
		if (CharacterComp)
		{
			CharacterComp->StopMovementImmediately();
			CharacterComp->DisableMovement();
			CharacterComp->SetComponentTickEnabled(false);
		}

		SetLifeSpan(10.0f);
		IsRagdoll = true;
	}

	GetMesh()->AddImpulseAtLocation(_bulletDirection * 50000, _hitpoint, _boneName);
}


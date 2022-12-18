// Fill out your copyright notice in the Description page of Project Settings.


#include "CCZombie.h"

#include "IBulletInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameSession.h"
#include "Components/SphereComponent.h"

// Sets default values
ACCZombie::ACCZombie()
{

	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
	GetCharacterMovement()->MaxWalkSpeed = 60.0f;

	HeadCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Head Collider"));
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	HeadCollider->InitSphereRadius(20.0f);
	HeadCollider->AttachToComponent(GetMesh(), AttachmentRules, FName("Head"));
	HeadCollider->BodyInstance.SetCollisionProfileName("BlockAll");
	HeadCollider->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	HeadCollider->CanCharacterStepUpOn = ECB_No;
}

// Called when the game starts or when spawned
void ACCZombie::BeginPlay()
{
	Super::BeginPlay();

	HeadCollider->OnComponentHit.AddDynamic(this, &ACCZombie::OnHit);

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
	if (CurrentHealth < 0)
	{
		Destroy();
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
	if (!HasAuthority())
	{
		Server_TakeDamage(_amount);
	}
	else
	{
		CurrentHealth -= _amount;
		if (CurrentHealth < 0)
		{
			OnDeath();
		}
	}
}

void ACCZombie::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr))
	{
		if (IIBulletInterface* bullet = Cast<IIBulletInterface>(OtherActor))
		{
			TakeDamage(MaxHealth);
		}
	}
}

void ACCZombie::OnDeath()
{
	Destroy();
}


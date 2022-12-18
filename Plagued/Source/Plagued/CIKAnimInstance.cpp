// Fill out your copyright notice in the Description page of Project Settings.


#include "CIKAnimInstance.h"
#include "PlaguedCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Net/UnrealNetwork.h"


void UCIKAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character)
	{
		AimTimeline.TickTimeline(DeltaSeconds);
		
		IsAiming = Character->IsAiming;
		SpineRotationX = Character->SpineRotationX;
		RifleEquiped = Character->RifleEquiped;
		Default();

		if (IsAiming)
		{
			SetSightTransform();
			SetRelativeHandTransform();

			AimTimeline.Play();
		}
		else
		{
			AimTimeline.Reverse();
		}
	}
}

void UCIKAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (APlaguedCharacter* character = Cast<APlaguedCharacter>(TryGetPawnOwner()))
	{
		SetCharacter(character);
		
		// Make 0.3 second timer and then call functions
		FTimerHandle TSetSightTransform;
		FTimerHandle TSetRelativeHandTransform;
		GetWorld()->GetTimerManager().SetTimer(TSetSightTransform, this, &UCIKAnimInstance::SetSightTransform, 0.3f, false);
		GetWorld()->GetTimerManager().SetTimer(TSetRelativeHandTransform, this, &UCIKAnimInstance::SetRelativeHandTransform, 0.3f, false);
	}

	if (AimCurveFloat)
	{
		FOnTimelineFloat timelineProgress;
		timelineProgress.BindDynamic(this, &UCIKAnimInstance::SetAimRatio);
		AimTimeline.AddInterpFloat(AimCurveFloat, timelineProgress);
		AimTimeline.SetPlayRate(4);
	}
}

void UCIKAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCIKAnimInstance, AimRatio);
}

void UCIKAnimInstance::Default()
{
	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Length();

	auto currentAccelleration = MovementComponent->GetCurrentAcceleration();
	ShouldMove = currentAccelleration != FVector::Zero() && GroundSpeed > 3.0f;

	IsFalling = MovementComponent->IsFalling();
}

void UCIKAnimInstance::SetCharacter(APlaguedCharacter* character)
{
	Character = character;
	MovementComponent = Character->GetCharacterMovement();
}

void UCIKAnimInstance::SetSightTransform()
{
	FTransform camTransform = Character->GetFirstPersonCameraComponent()->GetComponentTransform();
	FTransform meshTransform = Character->GetMesh()->GetComponentTransform();
	FTransform relative = UKismetMathLibrary::MakeRelativeTransform(camTransform, meshTransform);

	FVector newSightLocation = relative.GetLocation();
	FVector forwardVec = relative.Rotator().Quaternion().GetForwardVector();
	forwardVec *= 15.0f;

	newSightLocation += forwardVec;

	SightTransform.SetLocation(newSightLocation);
	SightTransform.SetRotation(relative.Rotator().Quaternion());
	SightTransform.SetScale3D({1,1,1});

	if (Character && !Character->HasAuthority())
		UE_LOG(LogTemp, Warning, TEXT("SightTransform: %s"), *SightTransform.ToString());
}

void UCIKAnimInstance::SetRelativeHandTransform()
{
	if (Character->EquipedItem)
	{
		if (USkeletalMeshComponent* mesh = Cast<USkeletalMeshComponent>(Character->EquipedItem->GetComponentByClass(USkeletalMeshComponent::StaticClass())))
		{
			FTransform meshTransform = Character->GetMesh()->GetSocketTransform(FName("RightHand"));
			FTransform opticSocketTransform = mesh->GetSocketTransform(FName("S_Sight"));
			RelativeHandTransform = UKismetMathLibrary::MakeRelativeTransform(opticSocketTransform,meshTransform);
			
			if (Character && !Character->HasAuthority())
				UE_LOG(LogTemp, Warning, TEXT("RelativeHandTransform: %s"), *RelativeHandTransform.ToString());
		}
	}
	
}

void UCIKAnimInstance::SetAimRatio(float _ratio)
{
	AimRatio = _ratio;
}

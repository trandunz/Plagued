// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlaguedCharacter.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CIKAnimInstance.generated.h"

UCLASS()
class PLAGUED_API UCIKAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeBeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, Category= "Variables")
	class UCharacterMovementComponent* MovementComponent;
	
	UPROPERTY(BlueprintReadWrite, Category= "Variables")
	APlaguedCharacter* Character;

	UPROPERTY(BlueprintReadWrite, Category= "Variables")
	FVector Velocity;

	UPROPERTY(BlueprintReadWrite, Category= "Variables")
	float GroundSpeed;
	
	UPROPERTY(BlueprintReadWrite, Category= "Variables")
	bool ShouldMove;

	UPROPERTY(BlueprintReadWrite, Category= "Variables")
	bool IsFalling;

	UPROPERTY(BlueprintReadWrite, Category= "Variables")
	bool IsAiming;

	UPROPERTY(Replicated, BlueprintReadWrite, Category= "Variables")
	float AimRatio;

	UPROPERTY(BlueprintReadWrite, Category= "Variables")
	FRotator SpineRotationX;

	UPROPERTY(BlueprintReadWrite, Category= "Variables")
	float SpineRotationAlpha = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category= "Variables")
	bool RifleEquiped;

	UPROPERTY(BlueprintReadWrite, Category= "Variables")
	FTransform SightTransform;
	
	UPROPERTY(BlueprintReadWrite, Category= "Variables")
	FTransform RelativeHandTransform;
	
protected:
	void Default();
	void SetCharacter(class APlaguedCharacter* character);
	void SetSightTransform();
	void SetRelativeHandTransform();

	UFUNCTION()
	void SetAimRatio(float _ratio);
	
	FTimeline AimTimeline;
    UPROPERTY(EditAnywhere)
    UCurveFloat* AimCurveFloat;
};

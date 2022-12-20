// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMagComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAGUED_API UCMagComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	int8 AddAmmo(int8 _amount);

	UFUNCTION()
	void FillMag();

	UFUNCTION()
	void EjectRound();

	UFUNCTION()
	bool IsEmpty();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo;
};

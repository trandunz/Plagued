// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CMagComponent.h"
#include "Components/ActorComponent.h"
#include "CGunComponent.generated.h"

UENUM()
enum class EFireType
{
	SINGLE = 0,
	BURST,
	AUTO
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAGUED_API UCGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCMagComponent* CurrentMag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFireType FireType = EFireType::SINGLE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EFireType> AvailableFireTypes {EFireType::SINGLE};
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAmmoMag.generated.h"

UCLASS()
class PLAGUED_API ACAmmoMag : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAmmoMag();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCMagComponent* MagComponent;
};

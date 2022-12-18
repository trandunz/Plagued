// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IBulletInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIBulletInterface : public UInterface
{
	GENERATED_BODY()
};

class PLAGUED_API IIBulletInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	float Damage;
};

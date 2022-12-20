// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CMagComponent.h"
#include "UObject/Interface.h"
#include "IGunInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIGunInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PLAGUED_API IIGunInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	APlayerController* InstigatorController;
	
	virtual void Fire();
	virtual void Reload();
	virtual void SetShotsFiredBasedOnFireType();
	virtual void ReleaseMouse();
	virtual void ChangeFireType();
	virtual UCMagComponent* GetCurrentMag();

	virtual class UCGunComponent* GetGunComponent();
};

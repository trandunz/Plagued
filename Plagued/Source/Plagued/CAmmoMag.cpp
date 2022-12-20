// Fill out your copyright notice in the Description page of Project Settings.


#include "CAmmoMag.h"
#include "Components/StaticMeshComponent.h"
#include "CMagComponent.h"

// Sets default values
ACAmmoMag::ACAmmoMag()
{
	PrimaryActorTick.bCanEverTick = true;
	MagComponent = CreateDefaultSubobject<UCMagComponent>(FName("Mag Component"));
	Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}


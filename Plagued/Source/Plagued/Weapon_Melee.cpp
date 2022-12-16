// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Melee.h"

// Sets default values
AWeapon_Melee::AWeapon_Melee()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AWeapon_Melee::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon_Melee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


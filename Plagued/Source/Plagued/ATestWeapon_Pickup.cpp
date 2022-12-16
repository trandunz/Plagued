// Fill out your copyright notice in the Description page of Project Settings.


#include "ATestWeapon_Pickup.h"

// Sets default values
AATestWeapon_Pickup::AATestWeapon_Pickup()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	RootComponent = Mesh;

	ItemData = CreateDefaultSubobject<UItemData>(TEXT("Item_Data"));
}

// Called when the game starts or when spawned
void AATestWeapon_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AATestWeapon_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AATestWeapon_Pickup::LookAt()
{
	if (ItemData)
		return ItemData->LookAt();

	return {};
}

void AATestWeapon_Pickup::Interact()
{
	if (ItemData)
		ItemData->Interact();
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CItemPickup.h"
#include "ItemData.h"

// Sets default values
ACItemPickup::ACItemPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	RootComponent = Mesh;

	ItemData = CreateDefaultSubobject<UItemData>(TEXT("ItemData"));
}

FString ACItemPickup::LookAt()
{
	if (ItemData)
		return ItemData->LookAt();

	return {};
}

void ACItemPickup::Interact()
{
	if (ItemData)
		ItemData->Interact();
}


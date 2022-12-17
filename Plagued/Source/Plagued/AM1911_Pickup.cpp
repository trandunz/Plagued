// Fill out your copyright notice in the Description page of Project Settings.


#include "AM1911_Pickup.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AAM1911_Pickup::AAM1911_Pickup()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	ItemData = CreateDefaultSubobject<UItemData>(TEXT("Item_Data"));
}

// Called when the game starts or when spawned
void AAM1911_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAM1911_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AAM1911_Pickup::LookAt()
{
	if (ItemData)
		return ItemData->LookAt();

	return {};
}

void AAM1911_Pickup::Interact()
{
	if (ItemData)
		ItemData->Interact();
}


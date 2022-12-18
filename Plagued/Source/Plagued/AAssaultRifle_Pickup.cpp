#include "AAssaultRifle_Pickup.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AAAssaultRifle_Pickup::AAAssaultRifle_Pickup()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	ItemData = CreateDefaultSubobject<UItemData>(TEXT("Item_Data"));
}

void AAAssaultRifle_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAAssaultRifle_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AAAssaultRifle_Pickup::LookAt()
{
	if (ItemData)
		return ItemData->LookAt();

	return {};
}

void AAAssaultRifle_Pickup::Interact()
{
	if (ItemData)
		ItemData->Interact();
}



#include "ItemData.h"
#include "AC_InventorySystem.h"

UItemData::UItemData()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}


// Called when the game starts
void UItemData::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->SetReplicates(true);
}


// Called every frame
void UItemData::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FString UItemData::LookAt()
{
	return "Pickup (" + ItemID.GetRow<FItemStruct>(ItemID.RowName.GetPlainNameString())->Name.ToString() + ")";
}

void UItemData::Interact()
{
	if (Quantity <= 0)
	{
		GetOwner()->Destroy();
	}
}

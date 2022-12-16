
#include "AC_InventorySystem.h"
#include "Net/UnrealNetwork.h"

UAC_InventorySystem::UAC_InventorySystem()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
	Content.Init({}, InventorySize);
}

void UAC_InventorySystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAC_InventorySystem, Content);
}


// Called when the game starts
void UAC_InventorySystem::BeginPlay()
{
	Super::BeginPlay();
	
}


int32 UAC_InventorySystem::FindSlot(FName _id)
{
	for (int i = 0; i < Content.Num(); i++)
	{
		if (_id == Content[i].ItemID)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Matching Item ID"));
			if (Content[i].Quantity < GetMaxStackSize(_id))
			{
				UE_LOG(LogTemp, Warning, TEXT("Existing Slot Found : %s"), *FString::FromInt(i));
				return i;
			}
		}
	}

	return -1;
}

int32 UAC_InventorySystem::GetMaxStackSize(FName _id)
{
	if (ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Item Data Refrence"));
		if (FItemStruct* item = ItemData->FindRow<FItemStruct>(_id, nullptr))
		{
			UE_LOG(LogTemp, Warning, TEXT("Max Stack Size : %s"), *FString::FromInt(item->StackSize));
			return item->StackSize;
		}
	}

	return -1;
}

void UAC_InventorySystem::AddToStack(int32 _index, int32 _quantity)
{
	Content[_index].Quantity += _quantity;
}

bool UAC_InventorySystem::AnyEmptySlots()
{
	for (auto Element : Content)
	{
		if (Element.Quantity == 0)
		{
			return true;
		}
	}

	return false;
}

void UAC_InventorySystem::CreateNewStack(FName _id, int32 _quantity)
{
	for (int i = 0; i < Content.Num(); i++)
	{
		if (Content[i].Quantity == 0)
		{
			Content[i].ItemID = _id;
			Content[i].Quantity = _quantity;
			break;
		}
	}
}

// Called every frame
void UAC_InventorySystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int32 UAC_InventorySystem::AddToInventory(FName _id, int32 _quantity)
{
	int32 startQuantity = _quantity;
	
	while(startQuantity > 0)
	{
		int32 slot = FindSlot(_id);
		if (slot != -1)
		{
			AddToStack(slot, 1);
			startQuantity--;
		}
		else if (AnyEmptySlots())
		{
			CreateNewStack(_id, 1);
			startQuantity--;
		}
		else
		{
			break;
		}
	}

	return startQuantity;
}

int32 UAC_InventorySystem::AddToInventory(UItemData* _item)
{
	while(_item->Quantity > 0)
	{
		int32 slot = FindSlot(_item->ItemID.RowName);
		if (slot != -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Slot Found"));
			AddToStack(slot, 1);
			_item->Quantity--;
		}
		else if (AnyEmptySlots())
		{
			UE_LOG(LogTemp, Warning, TEXT("Make New Stack"));
			CreateNewStack(_item->ItemID.RowName, 1);
			_item->Quantity--;
		}
		else
		{
			break;
		}
	}

	return _item->Quantity;
}


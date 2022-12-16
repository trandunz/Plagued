// Fill out your copyright notice in the Description page of Project Settings.


#include "CW_InventoryGrid.h"
#include "AC_InventorySystem.h"

void UCW_InventoryGrid::ConstructInventory()
{
	if (InventorySystem)
	{
		for (auto Element : InventorySystem->Content)
		{
			APlayerController* controller = GetOwningPlayer<APlayerController>();
			check(controller);
			ItemSlot = CreateWidget<UCW_ItemSlot>(controller, ItemSlotClass);
			ItemSlot->Quantity = Element.Quantity;
			ItemSlot->ItemID = Element.ItemID;
			Grid->AddChildToWrapBox(ItemSlot);
		}
	}
}

void UCW_InventoryGrid::NativePreConstruct()
{
	Super::NativePreConstruct();

	ConstructInventory();
}

void UCW_InventoryGrid::DisplayInventory(UAC_InventorySystem* _inventory)
{
	if (Grid)
	{
		InventorySystem = _inventory;
		Grid->ClearChildren();
		ConstructInventory();
	}
}

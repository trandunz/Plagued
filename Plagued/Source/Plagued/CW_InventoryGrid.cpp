// Fill out your copyright notice in the Description page of Project Settings.


#include "CW_InventoryGrid.h"
#include "AC_InventorySystem.h"
#include "PlaguedCharacter.h"
#include "Components/TextBlock.h"

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
			ItemSlot->SetupTooltip();
			ItemGrid->AddChildToWrapBox(ItemSlot);
		}
	}
}

void UCW_InventoryGrid::NativePreConstruct()
{
	Super::NativePreConstruct();

	ConstructInventory();
}

void UCW_InventoryGrid::NativeConstruct()
{
	Super::NativeConstruct();
	DisplayInventory();
}

void UCW_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InventoryTitle->SetText(FText::FromString(Title));
}

void UCW_InventoryGrid::DisplayInventory()
{
	if (UAC_InventorySystem* inventory = Cast<APlaguedCharacter>(GetOwningPlayer<APlayerController>()->GetCharacter())->InventorySystem)
	{
		if (ItemGrid)
		{
			InventorySystem = inventory;
			ItemGrid->ClearChildren();
			InventoryTitle->SetText(FText::FromString(Title));
			ConstructInventory();
		}
	}
}

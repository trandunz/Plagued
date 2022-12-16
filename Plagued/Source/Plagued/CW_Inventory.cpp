// Fill out your copyright notice in the Description page of Project Settings.


#include "CW_Inventory.h"

#include "PlaguedCharacter.h"

void UCW_Inventory::NativePreConstruct()
{
	Super::NativePreConstruct();

	
}

void UCW_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	if (Grid)
	{
		if (UAC_InventorySystem* inventory = Cast<APlaguedCharacter>(GetOwningPlayer<APlayerController>()->GetCharacter())->InventorySystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Show Inventory"));
			Grid->DisplayInventory(inventory);
		}
	}
}

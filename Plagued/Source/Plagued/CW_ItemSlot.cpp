// Fill out your copyright notice in the Description page of Project Settings.


#include "CW_ItemSlot.h"

#include <string>

#include "PlaguedCharacter.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UCW_ItemSlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ItemData && Quantity > 0)
	{
		if (FItemStruct* item = ItemData->FindRow<FItemStruct>(ItemID, nullptr))
		{
			Thumbnail->SetBrushFromTexture(item->Thumbnail);
			QuantityText->SetText(FText::AsNumber(Quantity));
			
			QuantityBox->SetVisibility(ESlateVisibility::Visible);
			Thumbnail->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			QuantityBox->SetVisibility(ESlateVisibility::Hidden);
			Thumbnail->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		QuantityBox->SetVisibility(ESlateVisibility::Hidden);
		Thumbnail->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCW_ItemSlot::EquipItem()
{
	if (Quantity > 0)
	{
		if (FItemStruct* item = ItemData->FindRow<FItemStruct>(ItemID, nullptr))
		{
			if (APlayerController* controller = GetOwningPlayer<APlayerController>())
			{
				if (APlaguedCharacter* character = Cast<APlaguedCharacter>(controller->GetCharacter()))
				{
					UE_LOG(LogTemp, Warning, TEXT("Equip Item From Inventory"));
					character->EquipItem(item->ItemClass);
				}
			}
		}
	}
}

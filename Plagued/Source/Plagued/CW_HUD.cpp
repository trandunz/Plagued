// Fill out your copyright notice in the Description page of Project Settings.


#include "CW_HUD.h"

#include <string>

#include "Components/Image.h"

void UCW_HUD::SetSlot(UTexture2D* _texture, int32 _index)
{
	UE_LOG(LogClass, Warning, TEXT("Changed Slot Icon: %i"), _index);
	
	switch(_index)
	{
	case 0:
		{
			Slot_Item->SetBrushFromTexture(_texture);
			break;
		}
	case 1:
		{
			Slot_Item_One->SetBrushFromTexture(_texture);
			break;
		}
	case 2:
		{
			Slot_Item_Two->SetBrushFromTexture(_texture);
			break;
		}
	case 3:
		{
			Slot_Item_Three->SetBrushFromTexture(_texture);
			break;
		}
	case 4:
		{
			Slot_Item_Four->SetBrushFromTexture(_texture);
			break;
		}
	case 5:
		{
			Slot_Item_Five->SetBrushFromTexture(_texture);
			break;
		}
	case 6:
		{
			Slot_Item_Six->SetBrushFromTexture(_texture);
			break;
		}
	default:
		break;
	}
}

void UCW_HUD::ShowInteractText(bool _show)
{
	if (_show)
	{
		InteractText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCW_HUD::ShowInteractText(bool _show, FString _message)
{
	InteractText->SetText(FText::FromString(_message));
	if (_show)
	{
		InteractText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractText->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGunComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CW_HUD.generated.h"

/**
 * 
 */
UCLASS()
class PLAGUED_API UCW_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSlot(UTexture2D* _texture, int32 _index);
	void ShowInteractText(bool _show);
	void ShowInteractText(bool _show, FString _message);

	void ShowAmmoText(bool _show);
	void ShowAmmoText(int32 _currentAmmo, int32 _clipSize, int32 _additionalMags);

	void ShowFireType(bool _show);
	void ShowFireType(bool _show, EFireType _fireType);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Slot_Item;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Slot_Item_One;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Slot_Item_Two;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Slot_Item_Three;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Slot_Item_Four;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Slot_Item_Five;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Slot_Item_Six;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InteractText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* AmmoCount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MagCount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* FireTypeText;
};

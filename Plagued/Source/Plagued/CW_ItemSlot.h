// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AC_InventorySystem.h"
#include "Blueprint/UserWidget.h"
#include "CInventoryTooltip.h"
#include "CW_ItemSlot.generated.h"

UCLASS()
class PLAGUED_API UCW_ItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativePreConstruct() override;

	void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void EquipItem();

	UFUNCTION(BlueprintCallable)
	void SetupTooltip();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCInventoryTooltip> ToolTipWidgetClass;
	UPROPERTY()
	UCInventoryTooltip* TooltipWidgetPointer;
	
	UPROPERTY(meta = (BindWidget))
	class UBorder* MasterBorder;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemData;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Thumbnail;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* QuantityText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USizeBox* QuantityBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;
};

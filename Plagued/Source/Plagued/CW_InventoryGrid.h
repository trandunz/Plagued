// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AC_InventorySystem.h"
#include "CW_ItemSlot.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"

#include "CW_InventoryGrid.generated.h"

/**
 * 
 */
UCLASS()
class PLAGUED_API UCW_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

	void ConstructInventory();
public:
	void NativePreConstruct() override;
	void NativeConstruct() override;
	void NativeOnInitialized() override;

	void DisplayInventory();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UCW_ItemSlot> ItemSlotClass;

	UPROPERTY()
	UCW_ItemSlot* ItemSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAC_InventorySystem* InventorySystem;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* ItemGrid;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InventoryTitle;

	UPROPERTY(EditAnywhere)
	FString Title;
};

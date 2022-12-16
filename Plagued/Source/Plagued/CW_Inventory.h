// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CW_InventoryGrid.h"
#include "Blueprint/UserWidget.h"
#include "CW_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class PLAGUED_API UCW_Inventory : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCW_InventoryGrid* Grid;
};

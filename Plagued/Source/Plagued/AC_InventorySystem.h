// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Item_Data.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "AC_InventorySystem.generated.h"

USTRUCT(BlueprintType)
struct FSlotStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName ItemID;
	UPROPERTY(EditAnywhere)
	int32 Quantity;
};

USTRUCT(BlueprintType)
struct FItemStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	FText Description;
	UPROPERTY(EditAnywhere)
	UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditAnywhere)
	int32 StackSize;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAGUED_API UAC_InventorySystem : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UAC_InventorySystem();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	TSubclassOf<AActor>* TryGetItem(FString _rowName);
	
	UPROPERTY(Replicated, EditAnywhere)
	TArray<FSlotStruct> Content;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int InventorySize = 14;

	UPROPERTY(EditAnywhere)
	UDataTable* ItemData;

	int32 FindSlot(FName _id);
	int32 GetMaxStackSize(FName _id);
	void AddToStack(int32 _index, int32 _quantity);
	bool AnyEmptySlots();
	void CreateNewStack(FName _id, int32 _quantity);
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int32 AddToInventory(FName _id, int32 _quantity);
	int32 AddToInventory(UItemData* _item);
};

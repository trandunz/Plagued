// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "ATestItem.generated.h"

UCLASS()
class PLAGUED_API AATestItem : public AActor, public IIInteractInterface
{
	GENERATED_BODY()
	
public:	
	AATestItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual FString LookAt() override;

	virtual void Interact() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere)
	UItemData* ItemData;
};
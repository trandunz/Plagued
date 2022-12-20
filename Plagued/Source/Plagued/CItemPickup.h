// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractInterface.h"
#include "GameFramework/Actor.h"
#include "CItemPickup.generated.h"

UCLASS()
class PLAGUED_API ACItemPickup : public AActor, public IIInteractInterface
{
	GENERATED_BODY()
	
public:	
	ACItemPickup();
	
	virtual FString LookAt() override;

	virtual void Interact() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere)
	UItemData* ItemData;
};

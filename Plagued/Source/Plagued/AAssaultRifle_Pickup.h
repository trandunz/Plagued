// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractInterface.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "AAssaultRifle_Pickup.generated.h"

UCLASS()
class PLAGUED_API AAAssaultRifle_Pickup : public AActor, public IIInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAAssaultRifle_Pickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FString LookAt() override;

	virtual void Interact() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere)
	UItemData* ItemData;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Base.h"

#include "CW_HUD.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BoxComponent.h"
#include "PlaguedCharacter.h"

// Sets default values
APickup_Base::APickup_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void APickup_Base::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickup_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup_Base::Pickup(APlaguedCharacter* _character)
{
	Destroy();
}

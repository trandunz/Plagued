#include "ATestItem.h"
#include "ItemData.h"


AATestItem::AATestItem()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	RootComponent = Mesh;

	ItemData = CreateDefaultSubobject<UItemData>(TEXT("Item_Data"));
}

void AATestItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AATestItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AATestItem::LookAt()
{
	if (ItemData)
		return ItemData->LookAt();

	return {};
}

void AATestItem::Interact()
{
	if (ItemData)
		ItemData->Interact();
}


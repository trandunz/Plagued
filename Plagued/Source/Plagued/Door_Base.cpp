// Fill out your copyright notice in the Description page of Project Settings.


#include "Door_Base.h"

// Sets default values
ADoor_Base::ADoor_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	Frame->SetupAttachment(RootComponent);

	Pivot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pivot"));
	Pivot->SetupAttachment(Frame);
	Pivot->SetRelativeLocation({-50.0f, 0.0f, 0.0f});
	
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(Pivot);
	Door->SetRelativeLocation({50.0f, 0.0f, 0.0f});
	
	Handle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	Handle->SetupAttachment(Door);
}

void ADoor_Base::Interact(FVector _instigatorForward)
{
	if (Closed)
	{
		float dotProduct = FVector::DotProduct((_instigatorForward - GetActorLocation()).GetSafeNormal(), GetActorRightVector());
		OpenAngle = dotProduct >= 0 ? -90.0f : 90.0f;
		Timeline.Play();
	}
	else
	{
		Timeline.Reverse();
	}
	Closed = !Closed;
}

void ADoor_Base::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void ADoor_Base::BeginPlay()
{
	Super::BeginPlay();
	if (CurveFloat)
	{
		FOnTimelineFloat timelineProgress;
		timelineProgress.BindDynamic(this, &ADoor_Base::OpenDoor);
		Timeline.AddInterpFloat(CurveFloat, timelineProgress);
	}
}

// Called every frame
void ADoor_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ADoor_Base::OpenDoor(float _ratio)
{
	UE_LOG(LogTemp, Warning, TEXT("Open Door"));
	FRotator rotation = FRotator(0.0f, OpenAngle * _ratio, 0.0f);
	Pivot->SetRelativeRotation(rotation);
}


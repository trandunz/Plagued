// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshCapture3D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
AMeshCapture3D::AMeshCapture3D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	MeshPlaceholder = CreateDefaultSubobject<USceneComponent>(TEXT("Mesh_Placeholder"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring_Arm"));
	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene_Capture_Component_2D"));

	MeshPlaceholder->SetupAttachment(Mesh);
	SpringArm->SetupAttachment(MeshPlaceholder);
	SceneCaptureComponent2D->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AMeshCapture3D::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeshCapture3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


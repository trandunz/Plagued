// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IBulletInterface.h"
#include "GameFramework/Actor.h"
#include "C556x45.generated.h"

UCLASS()
class PLAGUED_API AC556x45 : public AActor, public IIBulletInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC556x45();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;
};

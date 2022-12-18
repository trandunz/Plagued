// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGunInterface.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "CAssaultRifle.generated.h"

UCLASS()
class PLAGUED_API ACAssaultRifle : public AActor, public IIGunInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAssaultRifle();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, Category = "Firing")
	UNiagaraSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Firing")
	float MFCoefStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BulletPrefab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UArrowComponent* Barrel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ShootMontage;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Attack();
	bool Server_Attack_Validate();
	void Server_Attack_Implementation();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Attack();
	bool Multi_Attack_Validate();
	void Multi_Attack_Implementation();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Fire() override;
	virtual void Reload() override;
};

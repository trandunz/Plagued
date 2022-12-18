#include "CAssaultRifle.h"
#include "Components/ArrowComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACAssaultRifle::ACAssaultRifle()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Barrel = CreateDefaultSubobject<UArrowComponent>(TEXT("Barrel"));
	Barrel->SetupAttachment(Mesh);

	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
}

void ACAssaultRifle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ACAssaultRifle::BeginPlay()
{
	Super::BeginPlay();
}

bool ACAssaultRifle::Server_Attack_Validate()
{
	return true;
}

void ACAssaultRifle::Server_Attack_Implementation()
{
	Multi_Attack();
}

bool ACAssaultRifle::Multi_Attack_Validate()
{
	return true;
}

void ACAssaultRifle::Multi_Attack_Implementation()
{
	CurrentAmmo--;
	
	if (ShootMontage)
	{
		if (Mesh->GetAnimInstance())
		{
			Mesh->GetAnimInstance()->Montage_Play(ShootMontage);
		}
	}
	if (MuzzleFlash)
	{
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFlash, Mesh, NAME_None, Mesh->GetSocketLocation("Muzzle"), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
		NiagaraComp->SetNiagaraVariableFloat(FString("StrengthCoef"), MFCoefStrength);
	}
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	}
}

void ACAssaultRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAssaultRifle::Fire()
{
	if (BulletPrefab && CurrentAmmo > 0)
	{
		if (InstigatorController)
		{
			// Spawn the projectile at the muzzle
			GetWorld()->SpawnActor<AActor>(BulletPrefab, Barrel->GetComponentTransform());
			
			Server_Attack();
		}
	}
}

void ACAssaultRifle::Reload()
{
	CurrentAmmo = MaxAmmo;
}


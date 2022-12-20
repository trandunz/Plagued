#include "CAssaultRifle.h"
#include "Components/ArrowComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "CGunComponent.h"
#include "PlaguedCharacter.h"
#include "AC_InventorySystem.h"
#include "Net/UnrealNetwork.h"

ACAssaultRifle::ACAssaultRifle()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Barrel = CreateDefaultSubobject<UArrowComponent>(TEXT("Barrel"));
	Barrel->SetupAttachment(Mesh);

	GunComponent = CreateOptionalDefaultSubobject<UCGunComponent>(TEXT("Gun Component"));
	
	if (GunComponent->CurrentMag)
	{
		GunComponent->CurrentMag->MaxAmmo = 30;
	}
}

void ACAssaultRifle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACAssaultRifle, GunComponent);
	DOREPLIFETIME(ACAssaultRifle, ShotsFiredPerPress);
}

void ACAssaultRifle::ReleaseMouse()
{
	SetShotsFiredBasedOnFireType();
}

void ACAssaultRifle::ChangeFireType()
{
	if (GunComponent)
	{
		for(int i = 0 ; i < GunComponent->AvailableFireTypes.Num() + 1; i++)
		{
			if (i == GunComponent->AvailableFireTypes.Num())
			{
				GunComponent->FireType = GunComponent->AvailableFireTypes[0];
				break;
			}
			else if (GunComponent->FireType != GunComponent->AvailableFireTypes[i] && static_cast<int8>(GunComponent->AvailableFireTypes[i]) > static_cast<int8>(GunComponent->FireType))
			{
				GunComponent->FireType = GunComponent->AvailableFireTypes[i];
				break;
			}
		}
		SetShotsFiredBasedOnFireType();
	}
}

UCMagComponent* ACAssaultRifle::GetCurrentMag()
{
	if (GunComponent)
	{
		return GunComponent->CurrentMag;
	}

	return nullptr;
}

void ACAssaultRifle::BeginPlay()
{
	Super::BeginPlay();

	SetShotsFiredBasedOnFireType();
}

void ACAssaultRifle::SetShotsFiredBasedOnFireType()
{
	if (GunComponent)
	{
		switch(GunComponent->FireType)
		{
		case EFireType::BURST:
			{
				ShotsFiredPerPress = 3;
				break;	
			}
		case EFireType::AUTO:
			{
				ShotsFiredPerPress = INT32_MAX;
				break;	
			}
		default:
			ShotsFiredPerPress = 1;
			break;
		}
	}
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
	if (GunComponent->CurrentMag)
	{
		GunComponent->CurrentMag->EjectRound();
	
		ShotsFiredPerPress--;
		
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
}

void ACAssaultRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAssaultRifle::Fire()
{
	if (GunComponent->CurrentMag)
	{
		if (BulletPrefab && !GunComponent->CurrentMag->IsEmpty() && ShotsFiredPerPress > 0)
		{
			if (InstigatorController)
			{
				// Spawn the projectile at the muzzle
				GetWorld()->SpawnActor<AActor>(BulletPrefab, Barrel->GetComponentTransform());
			
				Server_Attack();
			}
		}
	}
}

void ACAssaultRifle::Reload()
{
	if (GunComponent->CurrentMag)
	{
		GunComponent->CurrentMag->FillMag();
	}
	else
	{
		if (APlaguedCharacter* character = Cast<APlaguedCharacter>(GetNetOwningPlayer()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Cast Worked"));
			if (TSubclassOf<AActor>* magClassFromInventory = character->InventorySystem->TryGetItem("556x45_Mag"))
			{
				UE_LOG(LogTemp, Warning, TEXT("Mag Actor Worked"));
				if (UCMagComponent* mag = Cast<UCMagComponent>(*magClassFromInventory))
				{
					UE_LOG(LogTemp, Warning, TEXT("Equip New Mag"));
					GunComponent->CurrentMag = mag;
				}
			}
		}
		
	}
}

UCGunComponent* ACAssaultRifle::GetGunComponent()
{
	return GunComponent;
}


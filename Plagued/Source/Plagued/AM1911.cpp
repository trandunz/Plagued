#include "AM1911.h"

#include "Editor.h"
#include "PlaguedCharacter.h"
#include "Components/ArrowComponent.h"

// Sets default values
AAM1911::AAM1911()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Barrel = CreateDefaultSubobject<UArrowComponent>(TEXT("Barrel"));
	Barrel->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AAM1911::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AAM1911::Server_Attack_Validate()
{
	return true;
}

void AAM1911::Server_Attack_Implementation()
{
	Multi_Attack();
}

bool AAM1911::Multi_Attack_Validate()
{
	return true;
}

void AAM1911::Multi_Attack_Implementation()
{
	if (ShootMontage)
	{
		Mesh->GetAnimInstance()->Montage_Play(ShootMontage);
	}
}

// Called every frame
void AAM1911::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAM1911::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Attempt To Fire M1911"));
	if (BulletPrefab)
	{
		if (InstigatorController)
		{
			// Spawn the projectile at the muzzle
			GetWorld()->SpawnActor<AActor>(BulletPrefab, Barrel->GetComponentTransform());

			UE_LOG(LogTemp, Warning, TEXT("Fired M1911"));
			Server_Attack();
		}
	}
}


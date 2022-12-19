
#include "C556x45.h"

#include "CCZombie.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AC556x45::AC556x45()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(2.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->SetNotifyRigidBodyCollision(true);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;
	
	InitialLifeSpan = 3.0f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.01;

	Damage = 30.0f;
}

// Called when the game starts or when spawned
void AC556x45::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentHit.AddDynamic(this, &AC556x45::OnHit);
}

// Called every frame
void AC556x45::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC556x45::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Something"));
	if ((OtherActor != nullptr))
	{
		if (ACCZombie* zombie = Cast<ACCZombie>(OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Zombie Hit"));
			if (Hit.BoneName == FName("Head"))
			{
				zombie->TakeDamage(100, -Hit.ImpactNormal, Hit.ImpactPoint, Hit.BoneName);
			}
			else
			{
				zombie->TakeDamage(Damage, -Hit.ImpactNormal, Hit.ImpactPoint, Hit.BoneName);
			}
		}

		Destroy();
	}
}


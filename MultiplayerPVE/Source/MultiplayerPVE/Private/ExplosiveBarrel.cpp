// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "../Components/UserHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/AudioComponent.h"
// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	HealthComp = CreateDefaultSubobject<UUserHealthComponent>(TEXT("HealthComp"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = MeshComp;
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 300;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;
	ExplosionImpulse = 400;
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	HealthComp->OnHealthChanged.AddDynamic(this, &AExplosiveBarrel::OnHealthChanged);

}

void AExplosiveBarrel::OnHealthChanged(UUserHealthComponent* OwnerHealthComp,AActor* victim, float Health, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (bExplored) {
		return;
	}
	if (Health <= 0) {
		bExplored = true;
		OnRep_Expolded();
		FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
		MeshComp->AddImpulse(BoostIntensity, NAME_None, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		MeshComp->SetMaterial(0, ExplodedMaterial);
		if (!AudioComp->IsPlaying()) {
			AudioComp->Play();
		}
		TArray<AActor*> NoActor;
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 55, GetActorLocation(), 50, DamageType1, NoActor, this, nullptr, true, ECC_Pawn);
		RadialForceComp->FireImpulse();
	}

}

void AExplosiveBarrel::OnRep_Expolded()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	MeshComp->SetMaterial(0, ExplodedMaterial);
}


void AExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AExplosiveBarrel, bExplored);
}

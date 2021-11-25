// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWeaponGrenade.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/ProjectileMovementComponent.h" 
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"

AUserWeaponGrenade::AUserWeaponGrenade()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1;
	bThrow = false;
	bOverlap = false;
	bExplored = false;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 300;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;
	ExplosionImpulse = 400;
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
	bReplicates = true;
	//SetReplicates(true);
	SetReplicateMovement(true);
	/*SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Block);*/
	//ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));//生成组件Projectile
	//ProjectileMovementComponent->bAutoActivate = false;
}

void AUserWeaponGrenade::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)

{
	if (ROLE_Authority == GetLocalRole()) {
		bOverlap = true;
		UE_LOG(LogTemp, Log, TEXT("in explore"));
		if (!bThrow || bExplored) {
			return;
		}
		if (bThrow && Cast<APawn>(OtherActor)) {
			ExploreEffect();
		}

	}
	

}

void AUserWeaponGrenade::OnRep_Expolded()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	MeshComp->SetMaterial(0, ExplodedMaterial);
}

void AUserWeaponGrenade::ServerExplore_Implementation()
{
	MeshComp->SetMaterial(0, ExplodedMaterial);
	RadialForceComp->FireImpulse();
	TArray<AActor*> NoActor;
	//AController* AC = Cast<APawn>(this->GetOwner())->GetController();
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 55, GetActorLocation(), 100, DamageType, NoActor, this->GetOwner(), nullptr, true, ECC_Pawn);
	SetLifeSpan(5.0f);
	
}

void AUserWeaponGrenade::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AUserWeaponGrenade::OnOverlapBegin);
}

void AUserWeaponGrenade::ExploreEffect()
{
	bExplored = true;
	OnRep_Expolded();
	if (GetLocalRole() != ROLE_Authority) {
		ServerExplore();
		return;
	}

	/*FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
	MeshComp->AddImpulse(BoostIntensity, NAME_None, true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	*/
	if (!AudioComp->IsPlaying()) {
		AudioComp->Play();
	}
	MeshComp->SetMaterial(0, ExplodedMaterial);
	RadialForceComp->FireImpulse();
	TArray<AActor*> NoActor;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 55, GetActorLocation(), 50, DamageType, NoActor, this, nullptr, true, ECC_Pawn);
	SetLifeSpan(5.0f);
	
}

void AUserWeaponGrenade::Throw(FVector StartDirection, float Speed)
{
	//ProjectileMovementComponent->SetVelocityInLocalSpace(StartDirection * Speed);	//设置速度，因为是矢量所以乘向前的向量
	//ProjectileMovementComponent->Activate();
	MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetEnableGravity(true);
	MeshComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
	if(GetLocalRole()==ROLE_Authority)
	UE_LOG(LogTemp, Log, TEXT("THROW"));
	MeshComp->AddImpulse(StartDirection * Speed);
	/*FString message = FString::Printf(TEXT("%f m.s"), SphereComp->GetComponentVelocity().Size());*/
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT());
	bThrow = true;
	
}


void AUserWeaponGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bExplored && bOverlap && GetVelocity().Size() < 100)
	{
		ExploreEffect();
	}
}

void AUserWeaponGrenade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUserWeaponGrenade, bExplored);
	DOREPLIFETIME(AUserWeaponGrenade, bThrow);
	DOREPLIFETIME(AUserWeaponGrenade, bOverlap);
}


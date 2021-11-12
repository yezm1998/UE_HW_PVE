// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
AUserWeapon::AUserWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MuzzlePosition = CreateDefaultSubobject<USphereComponent>(TEXT("MuzzlePosition"));
	MuzzlePosition->SetupAttachment(MeshComponent);
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);

	ShootRange = 500.0;
	Damage = 25;
	RateOfFire = 300;
	MuzzleSocketName = TEXT("MuzzleSocket");
	ThisWeaponType = WeaponType::Dagger;
}

// Called when the game starts or when spawned
void AUserWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUserWeapon::Fire()
{
	if (ThisWeaponType == WeaponType::Dagger) {
		DaggerFire();
	}
	else if (ThisWeaponType == WeaponType::Gun) {
		AActor* MyPawn = GetOwner();
		FHitResult Hit;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyPawn);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		//Vector3 MuzzlePosition =
		FVector Rotation = MuzzlePosition->GetForwardVector();
		FVector EndPosition = MuzzlePosition->GetComponentLocation() + Rotation * ShootRange;
		FVector TraceEndPoint = EndPosition;
		if (GetWorld()->LineTraceSingleByChannel(Hit, MuzzlePosition->GetComponentLocation(), EndPosition, ECC_Visibility, QueryParams)) {
			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor, Damage, Rotation, Hit, MyPawn->GetInstigatorController(), this, DamageType);
			if (HitEffect) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
			TraceEndPoint = Hit.ImpactPoint;
		}
		//DrawDebugLine(GetWorld(), MuzzlePosition->GetComponentLocation(), EndPosition, FColor::White, false, 0.2f, 0, 0.2f);
		if (MuzzleEffect) {
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
		}
		APawn* MyOwener = Cast<APawn>(GetOwner());
		if (MyOwener) {
			APlayerController* PC = Cast<APlayerController>(MyOwener->GetController());
			if (PC) {
				PC->ClientPlayCameraShake(FireShake);
			}
		}
		if (!AudioComp->IsPlaying()) {
			AudioComp->Play();
		}
		if (TraceEffect) {
			FVector MuzzleLoction = MeshComponent->GetSocketLocation(MuzzleSocketName);
			UParticleSystemComponent* TracerComp= UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, MuzzleLoction);
			if (TracerComp) {
				TracerComp->SetVectorParameter("BeamEnd", TraceEndPoint);
			}
		}
	}
	else {
		//TODO:手榴弹
		GrenadeFire();
	}
	
	//GetWorld()->TimeSeconds();
}

void AUserWeapon::StartFire()
{
	//防止点击过快超过射速
	//float FirstDelay = FMath::Max (LastTime + RateOfFire - GetWorld()->TimeSeconds , 0.0f);
	GetWorldTimerManager().SetTimer(FireTimer,this,&AUserWeapon::Fire, 60/ RateOfFire, true, 0);

}

void AUserWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
}

// Called every frame
void AUserWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


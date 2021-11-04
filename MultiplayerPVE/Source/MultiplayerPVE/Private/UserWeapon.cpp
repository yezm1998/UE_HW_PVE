// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AUserWeapon::AUserWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MuzzlePosition = CreateDefaultSubobject<USphereComponent>(TEXT("MuzzlePosition"));
	MuzzlePosition->SetupAttachment(MeshComponent);
	ShootRange = 500.0;
	Damage = 25;
	MuzzleSocketName = TEXT("MuzzleSocket");
}

// Called when the game starts or when spawned
void AUserWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUserWeapon::Fire()
{
	AActor* MyPawn = GetOwner();
	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyPawn);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	//Vector3 MuzzlePosition =
	FVector Rotation = MuzzlePosition->GetForwardVector();
	FVector EndPosition= MuzzlePosition->GetComponentLocation()+ Rotation * ShootRange;
	if (GetWorld()->LineTraceSingleByChannel(Hit, MuzzlePosition->GetComponentLocation(), EndPosition, ECC_Visibility, QueryParams)) {
		AActor* HitActor = Hit.GetActor();
		UGameplayStatics::ApplyPointDamage(HitActor, Damage, Rotation,Hit,MyPawn->GetInstigatorController(),this,DamageType);
		if (HitEffect) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
	}
	DrawDebugLine(GetWorld(), MuzzlePosition->GetComponentLocation(), EndPosition, FColor::White, false, 0.2f, 0, 0.2f);
	if (MuzzleEffect) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
	}
	APawn* MyOwener = Cast<APawn>(GetOwner());
	if (MyOwener) {
		APlayerController* PC=Cast<APlayerController>(MyOwener->GetController());
		if (PC) {
			PC->ClientPlayCameraShake(FireShake);
		}
	}
}

// Called every frame
void AUserWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWeaponDagger.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponType.h"

AUserWeaponDagger::AUserWeaponDagger() 
{
	RateOfFire = 60;
	ThisWeaponType = 0;
	BasicDamage = 100;
	ShootRange = 100;
}

void AUserWeaponDagger::ServerFireBox_Implementation()
{
	FireBox();
}

void AUserWeaponDagger::FireBox()
{
	AActor* MyPawn = GetOwner();
	if (MyPawn) {
		FHitResult Hit;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyPawn);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;
		//枪口位置
		FVector Rotation = MuzzlePosition->GetForwardVector();
		//视线位置保证射击屏幕中央
		FVector EyePosition; FRotator EyeRotation;
		MyPawn->GetActorEyesViewPoint(EyePosition, EyeRotation);
		//枪口前方  
		FVector EndPosition = MuzzlePosition->GetComponentLocation() + Rotation * ShootRange;
		//视野前方
		//FVector EndPosition = EyePosition + EyeRotation.Vector() * ShootRange;
		FVector TraceEndPoint = EndPosition;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyePosition, EndPosition, ECC_Visibility, QueryParams)) {
			AActor* HitActor = Hit.GetActor();
			//造成伤害
			UGameplayStatics::ApplyPointDamage(HitActor, BasicDamage, Rotation, Hit, MyPawn->GetInstigatorController(), this, DamageType);

			TraceEndPoint = Hit.ImpactPoint;
		}
	}
}

void AUserWeaponDagger::Fire()
{
	if (GetLocalRole() != ROLE_Authority) {
		ServerFireBox();
		return;
	}
	else {
		FireBox();
	}
}
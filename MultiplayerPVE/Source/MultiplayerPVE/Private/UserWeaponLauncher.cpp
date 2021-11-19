// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWeaponLauncher.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "UserWeaponGrenade.h"
AUserWeaponLauncher::AUserWeaponLauncher()
{
	RateOfFire = 120;
	ThrowWeaponSocket = "LaunchPoint";
}

void AUserWeaponLauncher::Fire()
{
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn) {
		StartThrow();
		EndThrow(MyPawn->GetControlRotation());
		//Cast<AUserCharacter>(MyPawn)->PlayAnimationByWeapon();
	}
}

void AUserWeaponLauncher::StartThrow()
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Grenade = GetWorld()->SpawnActor<AUserWeaponGrenade>(StarterThrowWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParam);
	Grenade->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, ThrowWeaponSocket);
}

void AUserWeaponLauncher::EndThrow(FRotator FV)
{
	//FDetachmentTransformRules DetachParam(AttachmentRules,false);
	
	Grenade->Throw(UKismetMathLibrary::GetForwardVector(FV), 300);
	Grenade = nullptr;
}
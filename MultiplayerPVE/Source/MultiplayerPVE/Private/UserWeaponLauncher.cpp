// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWeaponLauncher.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "UserWeaponGrenade.h"
#include "Net/UnrealNetwork.h"
AUserWeaponLauncher::AUserWeaponLauncher()
{
	RateOfFire = 120;
	ThrowWeaponSocket = "LaunchPoint";
}

void AUserWeaponLauncher::Fire()
{
	if (GetLocalRole() != ROLE_Authority) {
		ServerFireLauncher();
		return;
	}
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn) {
		StartThrow();
		EndThrow(MyPawn->GetControlRotation());
		//Cast<AUserCharacter>(MyPawn)->PlayAnimationByWeapon();
	}
	LastFireTime = GetWorld()->TimeSeconds;
}

void AUserWeaponLauncher::StartThrow()
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Grenade = GetWorld()->SpawnActor<AUserWeaponGrenade>(StarterThrowWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParam);
	Grenade->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, ThrowWeaponSocket);
	Grenade->SetOwner(this->GetOwner());
}

void AUserWeaponLauncher::EndThrow(FRotator FV)
{
	//FDetachmentTransformRules DetachParam(AttachmentRules,false);
	
	Grenade->Throw(UKismetMathLibrary::GetForwardVector(FV), 600);
	Grenade = nullptr;
}


void AUserWeaponLauncher::ServerFireLauncher_Implementation()
{
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn) {
		StartThrow();
		EndThrow(MyPawn->GetControlRotation());
		//Cast<AUserCharacter>(MyPawn)->PlayAnimationByWeapon();
	}
	//Fire();
}

bool AUserWeaponLauncher::ServerFireLauncher_Validate()
{
	return true;
}

void AUserWeaponLauncher::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUserWeaponLauncher, Grenade);
	
}

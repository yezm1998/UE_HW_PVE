// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWeapon.h"
#include "UserWeaponLauncher.generated.h"
class AUserWeaponGrenade;
/**
 * 
 */
UCLASS()
class MULTIPLAYERPVE_API AUserWeaponLauncher : public AUserWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AUserWeaponGrenade> StarterThrowWeaponClass;
	FName ThrowWeaponSocket;
	AUserWeaponGrenade* Grenade;
	void StartThrow();
	void EndThrow(FRotator FV);
public:
	AUserWeaponLauncher();
	void virtual Fire() override;
};

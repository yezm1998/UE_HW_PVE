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

	UPROPERTY(Replicated)
	AUserWeaponGrenade* Grenade;
	//UFUNCTION(Server)
	void StartThrow();
	//UFUNCTION(Server)
	void EndThrow(FRotator FV);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFireLauncher();
	
public:
	AUserWeaponLauncher();
	void virtual Fire() override;
};

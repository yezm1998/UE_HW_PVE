// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWeapon.h"
#include "UserWeaponDagger.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPVE_API AUserWeaponDagger : public AUserWeapon
{
	GENERATED_BODY()
protected:
	UFUNCTION(Server, Reliable)
	void ServerFireBox();
	
	void FireBox();
public:
	AUserWeaponDagger();
	void virtual Fire() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class WeaponType : uint8
{
			Dagger  UMETA(DisplayName="1"),
           Gun   UMETA(DisplayName="2"),
           Grenade   UMETA(DisplayName="3")
};

#define SurfaceDefault SurfaceType1
#define SurfaceFlesh SurfaceType2
#define FloorDefault SurfaceType3
#define WaterDefault SurfaceType4
#define CollisionWeapon ECC_GameTraceChannel1
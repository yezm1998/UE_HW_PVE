// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PVESaveGame.generated.h"

/**
 * 
 */

UCLASS()
class MULTIPLAYERPVE_API UPVESaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Basic")
		TMap<FString,int64> Leaderboard;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
public:
	UFUNCTION(BlueprintCallable,Category="Sort")
	void FindTopN(int64 n);
	UPVESaveGame();
};

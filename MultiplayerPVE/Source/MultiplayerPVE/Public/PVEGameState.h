// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PVEGameState.generated.h"

UENUM(BlueprintType)
enum class EnumWaveState:uint8 {
	PlayerToReady,
	WaitingToStart,
	WaveInPropress,
	WaitingToComplete,
	WaveComplete,
	GameOver,
};
/**
 * 
 */
UCLASS()
class MULTIPLAYERPVE_API APVEGameState : public AGameState
{
	GENERATED_BODY()
protected:
	UPROPERTY(Replicated, VisibleAnywhere,BlueprintReadOnly, Category = "GameState")
	int CountDownTime;
	UFUNCTION()
		void OnRep_WaveState(EnumWaveState OldState);
	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
		void WaveStateChanged(EnumWaveState NewState, EnumWaveState OldState);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
		EnumWaveState WaveState;
public:
	UFUNCTION()
		void SetNewTime(int NowTime);
	void SetWaveState(EnumWaveState NewWaveState);
	EnumWaveState GetWaveState();
	APVEGameState();
};

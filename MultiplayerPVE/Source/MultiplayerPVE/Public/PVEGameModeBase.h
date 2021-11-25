// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework\GameMode.h"
#include "PVEGameModeBase.generated.h"

class APlayerController;
/**
 * 
 */
enum class EnumWaveState:uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnActorKilledByUser, AActor*, VictimActor, AActor*, VictimActorController, AActor*, KillerActor, AActor*, KillerActorController);

UCLASS()
class MULTIPLAYERPVE_API APVEGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
protected:
	FTimerHandle TimerHandle_TimeCount;
	//int64 state;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameMode")
		float TimeWait;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int64 GameCountTime;
	int64 WaveNumber;
	int64 WaveEnemyNumber;
	TMap<APlayerController*, bool> MPC;
	TQueue<APlayerController*> QPC;
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
		void SpawnNewEnemy();
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
		void ShowResult();
	void SpawnWaveEnemys();
	APVEGameModeBase();
	void PrepareNextWave();
	void StartWare();
	void EndWare();
	bool CheckAnyPlayerAlive();
	void GameOver();
	void SetWaveState(EnumWaveState NewState);
	bool bGameState;
	void RestartDeadPlayers();
	/// <summary>
	/// pvp
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void PVPMode();
	void PVPCountDown();
	UFUNCTION(BlueprintImplementableEvent)
	void PVPEndGame();
	void RestartPlayerWithScends(float WaitTime, APlayerController* PC);
	void RestartPlayerGameMode();
public:
	virtual void StartPlay() override;
	//virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
		FOnActorKilledByUser OnActorKilled;
};

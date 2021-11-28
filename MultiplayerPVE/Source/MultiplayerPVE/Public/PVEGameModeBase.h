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
	FTimerHandle TimerHandle_EnemyWave;
	FTimerHandle TimerHandle_TimeCount;
	//int64 state;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameMode")
		float TimeWait;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int64 GameCountTime;
	//剩余波数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int64 WaveNumber;
	int64 WaveEnemyNumber;
	int64 Restart;
	//是否游戏结束
	bool bEnd;
	//是否生成结束
	bool bFinish;
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
		void SpawnNewMeleeEnemy();
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
		void SpawnNewBossEnemy();
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
		void SpawnNewRemoteEnemy();
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
		void ShowResult(bool Success);
	void SpawnWaveEnemys();
	APVEGameModeBase();
	//PVE mode
	UFUNCTION(BlueprintCallable)
		void PVEMode();
	void PrepareNextWave();
	void StartWare();
	void EndWare();
	bool CheckAnyPlayerAlive();
	void GameOver(bool Success);
	void SetWaveState(EnumWaveState NewState);
	EnumWaveState GetWaveState();
	bool bGameState;
	void RestartDeadPlayers();
	UFUNCTION(BlueprintImplementableEvent)
	bool CheckNPCAlive();
	UFUNCTION(BlueprintImplementableEvent)
	bool CheckAllAI();
	void PrepareFirstWave();
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
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
		FOnActorKilledByUser OnActorKilled;
	UPROPERTY(BlueprintReadOnly,Category="Ready")
	int PrepareTime;
};

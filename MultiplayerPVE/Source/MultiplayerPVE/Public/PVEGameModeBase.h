// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework\GameMode.h"
#include "PVEGameModeBase.generated.h"

/**
 * 
 */
enum class EnumWaveState:uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilledByUser, AActor*, VictimActor, AActor*, KillerActor, AActor*, KillerActorController);

UCLASS()
class MULTIPLAYERPVE_API APVEGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
protected:
	FTimerHandle TimerHandle_TimeCount;
	//int64 state;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameMode")
		float TimeWait;
	int64 GameCountTime;
	int64 WaveNumber;
	int64 WaveEnemyNumber;
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
public:
	virtual void StartPlay() override;
	//virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
		FOnActorKilledByUser OnActorKilled;
};

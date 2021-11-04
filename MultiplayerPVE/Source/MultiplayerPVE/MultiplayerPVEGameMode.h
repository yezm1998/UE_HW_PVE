// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerPVEGameMode.generated.h"

UCLASS(minimalapi)
class AMultiplayerPVEGameMode : public AGameMode
{
	GENERATED_BODY()
protected:
	FTimerHandle TimerHandle_TimeCount;
	//int64 state;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "GameMode")
	float TimeWait;
	int64 GameCountTime;
	int64 WaveNumber;
	int64 WaveEnemyNumber;
protected:
	UFUNCTION(BlueprintImplementableEvent,Category="GameMode")
		void SpawnNewEnemy();
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
		void ShowResult();
	void SpawnWaveEnemys();
	AMultiplayerPVEGameMode();
	void PrepareNextWave();
	void StartWare();
	void EndWare();
	bool CheckAnyPlayerAlive();
	void GameOver();
public:
	virtual void StartPlay() override;
	virtual void Tick(float DeltaTime) override;

};




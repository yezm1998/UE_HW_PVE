// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerPVEGameMode.h"
#include "MultiplayerPVECharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "PVESaveGame.h"
#include "TimerManager.h"
#include "UserCharacter.h"



AMultiplayerPVEGameMode::AMultiplayerPVEGameMode()
{
	WaveNumber = 0;
	TimeWait = 6.0f;
	GameCountTime = 60;
}

void AMultiplayerPVEGameMode::StartWare()
{
	++WaveNumber;
	WaveEnemyNumber = 1;// *WaveNumber;
	GetWorldTimerManager().SetTimer(TimerHandle_TimeCount, this, &AMultiplayerPVEGameMode::SpawnWaveEnemys, 1.0f, true, 0.0f);
}

void AMultiplayerPVEGameMode::EndWare()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeCount);
	PrepareNextWave();
}

bool AMultiplayerPVEGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator it=GetWorld()->GetPlayerControllerIterator();it;it++ )
	{
		APlayerController* PC = it->Get();
		if (PC && PC->GetPawn()) { 
			APawn* MyPawn = PC->GetPawn();
			AUserCharacter* Player=Cast<AUserCharacter>(MyPawn);
			if (ensure(Player) && Player->GetHealth() > 0) { //
				//UE_LOG(LogTemp, Log, TEXT("Health %f"), Player->GetHealth());
				return true;
			}
		}
	}
	//GameOver();
	return false;
}

void AMultiplayerPVEGameMode::GameOver()
{
	EndWare();
	ShowResult();
}

void AMultiplayerPVEGameMode::StartPlay()
{
	Super::StartPlay();
	PrepareNextWave();
}

void AMultiplayerPVEGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CheckAnyPlayerAlive();
}

void AMultiplayerPVEGameMode::PrepareNextWave()
{
	FTimerHandle TimerHandle_EnemyWave;
	GetWorldTimerManager().SetTimer(TimerHandle_EnemyWave, this, &AMultiplayerPVEGameMode::StartWare, TimeWait, false);
}

void AMultiplayerPVEGameMode::SpawnWaveEnemys()
{
	
	SpawnNewEnemy();
	--WaveEnemyNumber;
	if (WaveEnemyNumber<=0) {
		if (CheckAnyPlayerAlive())
			EndWare();
		else
			GameOver();
	}
}


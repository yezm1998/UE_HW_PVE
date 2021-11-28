// Fill out your copyright notice in the Description page of Project Settings.


#include "PVEGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "PVESaveGame.h"
#include "TimerManager.h"
#include "UserCharacter.h"
#include "UserPlayerState.h"
#include "PVEGameState.h"

APVEGameModeBase::APVEGameModeBase()
{
	GameStateClass = APVEGameState::StaticClass();
	PlayerStateClass = AUserPlayerState::StaticClass();
	WaveNumber = 5;
	Restart = 2;
	TimeWait = 15.0f;
	GameCountTime = 120;
	bGameState = true;
	bEnd = false;
	bFinish = false;
	PrepareTime = 5;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void APVEGameModeBase::StartWare()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_EnemyWave);
	--WaveNumber;
	WaveEnemyNumber = 1;// *WaveNumber;
	GetWorldTimerManager().SetTimer(TimerHandle_TimeCount, this, &APVEGameModeBase::SpawnWaveEnemys, 1.0f, true, 0.0f);
	SetWaveState(EnumWaveState::WaveInPropress);
}

void APVEGameModeBase::EndWare()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeCount);
	if (WaveNumber) {
		PrepareNextWave();
		SetWaveState(EnumWaveState::WaitingToComplete);
	}
	else {
		bEnd = true;
	}

}

void APVEGameModeBase::PrepareNextWave()
{
	
	GetWorldTimerManager().SetTimer(TimerHandle_EnemyWave, this, &APVEGameModeBase::StartWare, TimeWait, false);
	SetWaveState(EnumWaveState::WaitingToStart);
	// ? RestartDeadPlayers();
}

bool APVEGameModeBase::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; it++)
	{
		APlayerController* PC = it->Get();
		if (PC && PC->GetPawn()) {
			APawn* MyPawn = PC->GetPawn();
			AUserCharacter* Player = Cast<AUserCharacter>(MyPawn);
			if (!Player)return true;
			if (ensure(Player) && !Player->IsAI && Player->GetHealth() > 0 ) { //
				//UE_LOG(LogTemp, Log, TEXT("Health %f"), Player->GetHealth());
				return true;
			}
		}
	}
	//SetWaveState(EnumWaveState::WaveComplete);
	return false;
}

void APVEGameModeBase::GameOver(bool Success)
{
	//EndWare();
	bFinish = true;
	ShowResult(Success);
	SetWaveState(EnumWaveState::GameOver);
}



void APVEGameModeBase::SetWaveState(EnumWaveState NewState)
{
	APVEGameState* GS = GetGameState<APVEGameState>();
	if (ensureAlways(GS)) {
		GS->SetWaveState(NewState);
	}
}

EnumWaveState APVEGameModeBase::GetWaveState()
{
	APVEGameState* GS = GetGameState<APVEGameState>();
	if (ensureAlways(GS)) {
		 return GS->GetWaveState();
	}
	return EnumWaveState::WaitingToStart;
}

void APVEGameModeBase::RestartDeadPlayers()
{
	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; it++)
	{
		APlayerController* PC = it->Get();
		
		if (PC && PC->GetPawn() == nullptr) {
			if (Restart > 0) {
				RestartPlayer(PC);
				Restart--;
			}
			else {
				return;
			}
		}
	}
}

void APVEGameModeBase::StartPlay()
{
	Super::StartPlay();
	//PrepareNextWave();
}


void APVEGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bFinish)
		return;
	if (Restart < 1 && !CheckAnyPlayerAlive()) {
		GameOver(false);
	}
	if (!CheckNPCAlive()) {
		GameOver(false);
	}
	if (!CheckAllAI() && GetWaveState() == EnumWaveState::WaitingToStart) {
		StartWare();
	}
	if (bEnd && !CheckAllAI()) {
		GameOver(true);
	}
	RestartDeadPlayers();
	
}

void APVEGameModeBase::PVEMode()
{
	FTimerHandle TimerHandle_Temp;
	GetWorldTimerManager().SetTimer(TimerHandle_Temp, this, &APVEGameModeBase::PrepareFirstWave, 1, true);
}

void APVEGameModeBase::SpawnWaveEnemys()
{

	SpawnNewMeleeEnemy();
	SpawnNewBossEnemy();
	SpawnNewRemoteEnemy();
	--WaveEnemyNumber;
	if (WaveEnemyNumber <= 0) {
		EndWare();
	}
}

void APVEGameModeBase::PrepareFirstWave()
{
	if (!PrepareTime) {
		GetWorldTimerManager().ClearTimer(TimerHandle_TimeCount);
		StartWare();
		//PrepareNextWave();
	}
	PrepareTime--;
}

void APVEGameModeBase::PVPMode()
{
	//GetWorldTimerManager().SetTimer(TimerHandle_TimeCount, this, &ALaunch::CutTime, 1.0f, true, 1.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeCount, this, &APVEGameModeBase::PVPCountDown, 1, true, 1);
}

void APVEGameModeBase::PVPCountDown()
{
	if (!GameCountTime) {
		PVPEndGame();
		GetWorldTimerManager().ClearTimer(TimerHandle_TimeCount);
		return;
	}
	--GameCountTime;
	APVEGameState* GS = GetGameState<APVEGameState>();
	GS->SetNewTime(GameCountTime);
	RestartDeadPlayers();
	
}

void APVEGameModeBase::RestartPlayerWithScends(float WaitTime, APlayerController* PC)
{
	if (Restart) {
		--Restart;
		RestartPlayer(PC);
	}
	/*FTimerHandle TimerHandle_temp;
	GetWorldTimerManager().SetTimer(TimerHandle_temp, this, &APVEGameModeBase::RestartPlayerGameMode, WaitTime, false);
	RestartPlayer(PC);*/
}

void APVEGameModeBase::RestartPlayerGameMode()
{	
	/*APlayerController* PC =QPC.Peek();
	RestartPlayer(PC);
	MPC.Remove(PC);
	QPC.Pop();*/
	
}

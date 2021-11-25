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
	WaveNumber = 0;
	TimeWait = 5.0f;
	GameCountTime = 40;
	bGameState = true;
	/*PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;*/
}

void APVEGameModeBase::StartWare()
{
	++WaveNumber;
	WaveEnemyNumber = 1;// *WaveNumber;
	GetWorldTimerManager().SetTimer(TimerHandle_TimeCount, this, &APVEGameModeBase::SpawnWaveEnemys, 1.0f, true, 0.0f);
	SetWaveState(EnumWaveState::WaveInPropress);
}

void APVEGameModeBase::EndWare()
{
	if (bGameState && CheckAnyPlayerAlive()) {
		GetWorldTimerManager().ClearTimer(TimerHandle_TimeCount);
		PrepareNextWave();
	}
	
	if (bGameState && !CheckAnyPlayerAlive()) {
		GetWorldTimerManager().ClearTimer(TimerHandle_TimeCount);
		bGameState = false;
		//PrepareNextWave();
	}
	SetWaveState(EnumWaveState::WaitingToComplete);

}

void APVEGameModeBase::PrepareNextWave()
{
	FTimerHandle TimerHandle_EnemyWave;
	GetWorldTimerManager().SetTimer(TimerHandle_EnemyWave, this, &APVEGameModeBase::StartWare, TimeWait, false);
	SetWaveState(EnumWaveState::WaitingToStart);
	RestartDeadPlayers();
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
			if (ensure(Player) && Player->GetHealth() > 0 ) { //
				//UE_LOG(LogTemp, Log, TEXT("Health %f"), Player->GetHealth());
				return true;
			}
		}
	}
	SetWaveState(EnumWaveState::WaveComplete);
	return false;
}

void APVEGameModeBase::GameOver()
{
	EndWare();
	ShowResult();
	SetWaveState(EnumWaveState::GameOver);
}



void APVEGameModeBase::SetWaveState(EnumWaveState NewState)
{
	APVEGameState* GS = GetGameState<APVEGameState>();
	if (ensureAlways(GS)) {
		GS->SetWaveState(NewState);
	}
}

void APVEGameModeBase::RestartDeadPlayers()
{
	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; it++)
	{
		APlayerController* PC = it->Get();
		
		if (PC && PC->GetPawn() == nullptr) {
			/*QPC.Enqueue(PC);
			MPC.Add( PC, true );
			RestartPlayerWithScends(2, PC);*/
			RestartPlayer(PC);
		}
	}
}

void APVEGameModeBase::StartPlay()
{
	Super::StartPlay();
	//PrepareNextWave();
}


/*void APVEGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bGameState && !CheckAnyPlayerAlive()) {
		GameOver();
	}
}*/


void APVEGameModeBase::SpawnWaveEnemys()
{

	SpawnNewEnemy();
	--WaveEnemyNumber;
	if (WaveEnemyNumber <= 0) {
		if (CheckAnyPlayerAlive())
			EndWare();
		else
			GameOver();
		//EndWare();
	}
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
	
	FTimerHandle TimerHandle_temp;
	GetWorldTimerManager().SetTimer(TimerHandle_temp, this, &APVEGameModeBase::RestartPlayerGameMode, WaitTime, false);
	RestartPlayer(PC);
}

void APVEGameModeBase::RestartPlayerGameMode()
{	
	/*APlayerController* PC =QPC.Peek();
	RestartPlayer(PC);
	MPC.Remove(PC);
	QPC.Pop();*/
	
}

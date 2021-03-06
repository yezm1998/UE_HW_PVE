// Fill out your copyright notice in the Description page of Project Settings.


#include "PVEGameState.h"
#include "Net/UnrealNetwork.h"
void APVEGameState::OnRep_WaveState(EnumWaveState OldState)
{
	WaveStateChanged(WaveState, OldState);
}

void APVEGameState::SetNewTime(int NowTime)
{
	CountDownTime = NowTime;
}

void APVEGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APVEGameState, WaveState);
	DOREPLIFETIME(APVEGameState, CountDownTime);
	
}

void APVEGameState::SetWaveState(EnumWaveState NewWaveState)
{
	if (GetLocalRole() == ROLE_Authority) {
		EnumWaveState OldWaveState = WaveState;
		WaveState = NewWaveState;
		OnRep_WaveState(OldWaveState);
	}
}

EnumWaveState APVEGameState::GetWaveState()
{
	return WaveState;
}

APVEGameState::APVEGameState()
{
	WaveState = EnumWaveState::PlayerToReady;
}

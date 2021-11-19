// Fill out your copyright notice in the Description page of Project Settings.


#include "PVEGameState.h"
#include "Net/UnrealNetwork.h"
void APVEGameState::OnRep_WaveState(EnumWaveState OldState)
{
	WaveStateChanged(WaveState, OldState);
}

void APVEGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APVEGameState, WaveState);
}

void APVEGameState::SetWaveState(EnumWaveState NewWaveState)
{
	if (GetLocalRole() == ROLE_Authority) {
		EnumWaveState OldWaveState = WaveState;
		WaveState = NewWaveState;
		OnRep_WaveState(OldWaveState);
	}
}

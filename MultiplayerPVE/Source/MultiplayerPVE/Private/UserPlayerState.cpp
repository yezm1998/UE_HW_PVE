// Fill out your copyright notice in the Description page of Project Settings.


#include "UserPlayerState.h"
#include "Net/UnrealNetwork.h"
void AUserPlayerState::AddScore(float ScoreDelta)
{
	Score += ScoreDelta;
}

void AUserPlayerState::AddDiedNumber_Implementation(float ScoreDelta)
{
	
	NumberDied+= ScoreDelta;
}

void AUserPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUserPlayerState, NumberDied);
}

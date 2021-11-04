// Fill out your copyright notice in the Description page of Project Settings.


#include "PVESaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
UPVESaveGame::UPVESaveGame()
{
	SaveSlotName="Save";
	UserIndex = 1;
}

void UPVESaveGame::FindTopN(int64 n) {
	/*Leaderboard.Add(TEXT("Banana"),5);
	Leaderboard.Add(TEXT("Grapefruit"),6);
	Leaderboard.Add(TEXT("Pineapple"),7);*/
	Leaderboard.ValueSort([](const int64 A, const int64 B) {
		return A > B; // sort strings by length
		});
	for (auto& Elem : Leaderboard)
	{
		UE_LOG(LogTemp,Log,TEXT("%s,%d"),*Elem.Key,Elem.Value);
	}
	return;
}
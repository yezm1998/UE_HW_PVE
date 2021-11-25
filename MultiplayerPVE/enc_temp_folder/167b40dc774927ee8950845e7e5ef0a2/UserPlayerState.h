// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UserPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPVE_API AUserPlayerState : public APlayerState
{
	GENERATED_BODY()
protected:
	UPROPERTY(Replicated, VisibleAnywhere,BlueprintReadOnly, Category = "PlayerScore")
	int NumberDied=0;
public:
	bool bRevive=true;
	UFUNCTION(BlueprintCallable, Category = "PlayerScore")
	void AddScore(float ScoreDelta);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "PlayerScore")
	void AddDiedNumber(float ScoreDelta);
};

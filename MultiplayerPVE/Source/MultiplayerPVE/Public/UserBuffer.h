// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UserBuffer.generated.h"

UCLASS()
class MULTIPLAYERPVE_API AUserBuffer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUserBuffer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//持续时间
	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
		int32 PowerTime;
	//回复间隔时间
	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
		float PowerInterval;
	//每一次迭代调用函数
	UFUNCTION()
	void OnTickPowerUp();
	FTimerHandle TimeHandle;
public:	
	void ActivatePowerUp();
	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUp")
		void OnActivated();
	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUp")
		void OnExpired();
	//实现具体power内容
	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUp")
		void OnPowerUpTick();
};

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
	//����ʱ��
	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
		int32 PowerTime;
	//�ظ����ʱ��
	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
		float PowerInterval;
	//ÿһ�ε������ú���
	UFUNCTION()
	void OnTickPowerUp();
	FTimerHandle TimeHandle;
public:	
	void ActivatePowerUp();
	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUp")
		void OnActivated();
	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUp")
		void OnExpired();
	//ʵ�־���power����
	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUp")
		void OnPowerUpTick();
};

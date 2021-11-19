// Fill out your copyright notice in the Description page of Project Settings.


#include "UserBuffer.h"

// Sets default values
AUserBuffer::AUserBuffer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PowerInterval = 0;
	PowerTime = 0;
}

// Called when the game starts or when spawned
void AUserBuffer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUserBuffer::OnTickPowerUp()
{
	OnPowerUpTick();
	if (PowerTime == 0) {
		OnExpired();
		GetWorldTimerManager().ClearTimer(TimeHandle);
	}
	PowerTime--;
}

void AUserBuffer::ActivatePowerUp()
{
	if (PowerInterval>0) {
		GetWorldTimerManager().SetTimer(TimeHandle,this,&AUserBuffer::OnTickPowerUp,PowerInterval,true,0);
		OnActivated();
	}
	
}



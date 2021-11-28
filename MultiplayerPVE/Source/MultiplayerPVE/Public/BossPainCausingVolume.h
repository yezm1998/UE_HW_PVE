// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PainCausingVolume.h"
#include "BossPainCausingVolume.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPVE_API ABossPainCausingVolume : public AActor
{
	GENERATED_BODY()
protected:
	/** Damage done per second to actors in this volume when bPainCausing=true */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PainCausingVolume")
		float DamagePerSec;

	/** Type of damage done */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PainCausingVolume")
		TSubclassOf<UDamageType> DamageType;

	/** If pain causing, time between damage applications. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PainCausingVolume")
		float PainInterval;

	/** Controller that gets credit for any damage caused by this volume */
	UPROPERTY()
		class AController* DamageInstigator;

	/** Damage overlapping actors if pain-causing. */
	virtual void PainTimer();

	virtual void EndPlay();

	
	//End AActor Interface

	//Being PhysicsVolume Interface
	/** If bEntryPain is true, call CausePainTo() on entering actor. */
	virtual void ActorEnteredVolume(class AActor* Other);
	//End PhysicsVolume Interface

	/** damage overlapping actors if pain causing. */
	virtual void CausePainTo(class AActor* Other);

	/** Handle for efficient management of OnTimerTick timer */
	FTimerHandle TimerHandle_PainTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PainCausingVolume")
	class UBoxComponent* BoxComp;
public:
	ABossPainCausingVolume();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};

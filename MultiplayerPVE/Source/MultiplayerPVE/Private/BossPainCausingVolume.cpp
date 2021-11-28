// Fill out your copyright notice in the Description page of Project Settings.


#include "BossPainCausingVolume.h"
#include "Components/BoxComponent.h"

ABossPainCausingVolume::ABossPainCausingVolume()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	DamageType = UDamageType::StaticClass();
	DamagePerSec = 1.0f;
	PainInterval = 1.0f;
	bReplicates = true;

}


void ABossPainCausingVolume::EndPlay()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_PainTimer);
}

void ABossPainCausingVolume::ActorEnteredVolume(AActor* Other)
{
	//Super::ActorEnteredVolume(Other);
	if (Other->CanBeDamaged())
	{
		CausePainTo(Other);
	}

	// Start timer if none is active
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle_PainTimer))
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PainTimer, this, &ABossPainCausingVolume::PainTimer, PainInterval, true);
	}
}

void ABossPainCausingVolume::PainTimer()
{
	
		TSet<AActor*> TouchingActors;
		GetOverlappingActors(TouchingActors, APawn::StaticClass());

		for (AActor* const A : TouchingActors)
		{
			if (A && A->CanBeDamaged() && !A->IsPendingKill())
			{
				// @todo physicsVolume This won't work for other actor. Need to fix it properly
				APawn* PawnA = Cast<APawn>(A);
				if (PawnA)
				{
					CausePainTo(A);
				}
			}
		}

		// Stop timer if nothing is overlapping us
		if (TouchingActors.Num() == 0)
		{
			EndPlay();
			//GetWorldTimerManager().ClearTimer(TimerHandle_PainTimer);
		}
	
}

void ABossPainCausingVolume::CausePainTo(AActor* Other)
{
	if (DamagePerSec > 0.f)
	{
		TSubclassOf<UDamageType> DmgTypeClass = DamageType ? *DamageType : UDamageType::StaticClass();
		Other->TakeDamage(DamagePerSec * PainInterval, FDamageEvent(DmgTypeClass), DamageInstigator, this);
	}
}

void ABossPainCausingVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	UE_LOG(LogTemp, Log, TEXT("pain overlap"));
	ActorEnteredVolume(OtherActor);
}


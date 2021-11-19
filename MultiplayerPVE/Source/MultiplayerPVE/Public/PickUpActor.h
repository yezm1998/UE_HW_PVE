// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpActor.generated.h"
class USphereComponent;
class UDecalComponent;
class AUserBuffer;
UCLASS()
class MULTIPLAYERPVE_API APickUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDecalComponent* DecalComp;
	UPROPERTY(EditDefaultsOnly,Category="PickUpActor")
	TSubclassOf<AUserBuffer> PowerUpClass;
	void SpawnPickUpActor();
	AUserBuffer* PickUpInstance;
public:	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "UserBuffer.h"
// Sets default values

APickUpActor::APickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75);
	RootComponent = SphereComp;
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(SphereComp);
	DecalComp->SetRelativeRotation(FRotator(90, 0, 0));

}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();
	SpawnPickUpActor();
}

void APickUpActor::SpawnPickUpActor()
{
	if (PowerUpClass == nullptr) {

		return;
	}
	FActorSpawnParameters SpawnPickParam;
	SpawnPickParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PickUpInstance = GetWorld()->SpawnActor<AUserBuffer>(PowerUpClass, GetTransform(), SpawnPickParam);
}

void APickUpActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	UE_LOG(LogTemp, Log, TEXT("overlap"));
	if (PickUpInstance) {
		PickUpInstance->ActivatePowerUp();
		PickUpInstance = nullptr;
	}
}


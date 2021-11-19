// Fill out your copyright notice in the Description page of Project Settings.


#include "UserHealthComponent.h"
#include "PVEGameModeBase.h"
#include "Net/UnrealNetwork.h"
// Sets default values for this component's properties
UUserHealthComponent::UUserHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	DefaultHealth = 100.0f;
	SetIsReplicated(true);
	//SetReplicates(true);

}


// Called when the game starts
void UUserHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwnerRole() == ROLE_Authority) {
		AActor* MyOwner = GetOwner();
		if (MyOwner) {
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UUserHealthComponent::HandleAnyDamage);
		}
	}
	
	Health = DefaultHealth;
	
}

void UUserHealthComponent::HandleAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) 
{
	if (Health <= 0 || Damage <= 0) {
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	//UE_LOG(LogTemp, Log, TEXT("%f %f"), Health, Damage);
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

float UUserHealthComponent::GetHealth()
{
	return Health;
}

void UUserHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UUserHealthComponent, Health);

}

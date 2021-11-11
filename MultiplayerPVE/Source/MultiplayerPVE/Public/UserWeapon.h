// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponType.h"
#include "UserWeapon.generated.h"
class USkeletalMeshComponent;
class USphereComponent;
class UDamageType;
class UParticleSystem;

class UAudioComponent;

UCLASS()
class MULTIPLAYERPVE_API AUserWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUserWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="MeshComp")
	USkeletalMeshComponent* MeshComponent;
	
	float Damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire")
		float ShootRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		WeaponType ThisWeaponType;
	UPROPERTY(EditAnywhere, Category = "Fire")
		USphereComponent* MuzzlePosition;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
		FName MuzzleSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
		UParticleSystem* MuzzleEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
		UParticleSystem* HitEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		TSubclassOf<UCameraShake> FireShake;
	UFUNCTION(BlueprintImplementableEvent, Category = "FireAnimation")
		void DaggerFire();
	UFUNCTION(BlueprintImplementableEvent, Category = "FireAnimation")
		void GunFire();
	UFUNCTION(BlueprintImplementableEvent, Category = "FireAnimation")
		void GrenadeFire();
	UPROPERTY(VisibleAnywhere, Category = "Sound")
		UAudioComponent* AudioComp;
	virtual void Fire();
	FTimerHandle FireTimer;
	float RateOfFire;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	//UFUNCTION(BlueprintCallable, Category = "Weapon")
		
	//自动开火
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void StartFire();
	//停止开火
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void StopFire();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWeaponGrenade.generated.h"
class UProjectileMovementComponent;
class UStaticMeshComponent;
class URadialForceComponent;
class UParticleSystem;
class USphereComponent;

/**
 * 
 */

UCLASS()
class MULTIPLAYERPVE_API AUserWeaponGrenade : public AActor
{
	GENERATED_BODY()
public:
	AUserWeaponGrenade();
protected:
	/*UPROPERTY(VisibleAnywhere, Category = "sphere")
		UProjectileMovementComponent* ProjectileMovementComponent = nullptr;*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Components")
		USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		URadialForceComponent* RadialForceComp;
	UPROPERTY(ReplicatedUsing = OnRep_Expolded)
		bool bExplored;
	UFUNCTION()
		void OnRep_Expolded();
	UPROPERTY(EditDefaultsOnly, Category = "FX")
		float ExplosionImpulse;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
		UParticleSystem* ExplosionEffect;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
		UMaterialInterface* ExplodedMaterial;
	bool bThrow;
	virtual void BeginPlay() override;
	void ExploreEffect();
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	bool bOverlap;
public:
	UFUNCTION()
		void Throw(FVector StartDirection,float Speed);
	virtual void Tick(float DeltaTime) override;
};

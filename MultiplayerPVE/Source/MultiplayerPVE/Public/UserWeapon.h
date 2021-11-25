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
class AUserCharacter;
class UAnimationAsset;
class UAnimMontage;

//����켣����Ϣ
USTRUCT()
struct FHitScanTrace 
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface>  SurfaceType;
	UPROPERTY()
	FVector_NetQuantize  TraceEnd;
	UPROPERTY()
	FHitResult  HitResult;
	UPROPERTY()
	uint64  FireTime;
};

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickComp")
	USphereComponent* SphereComp;

	void PlayEffect(FVector TraceEndPoint);
	void PlayImpactEffect(EPhysicalSurface SurfaceType, FHitResult Hit);
	float BasicDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire")
		float ShootRange;
	
	UPROPERTY(EditAnywhere, Category = "Fire")
		USphereComponent* MuzzlePosition;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
		FName MuzzleSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
		UParticleSystem* MuzzleEffect;
	//�����Ч
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
		UParticleSystem* DefaultHitEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
		UParticleSystem* FleshBloodHitEffect;
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
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		UParticleSystem* TraceEffect;
	virtual void Fire();
	FTimerHandle FireTimer;
	float RateOfFire;

	//���������ÿ���
	UFUNCTION(Server,Reliable,WithValidation)
	void ServerFire();
	//�����·�ߴ��ݵ��ͷ���
	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;
	UFUNCTION()
	void OnRep_HitScanTrace();

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	//�������
	UPROPERTY(EditDefaultsOnly, Category = "FireAnim")
	class UAnimationAsset* Anim;
	UPROPERTY(EditDefaultsOnly, Category = "FireAnim")
	class UAnimMontage* WeaponMontage;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	//UFUNCTION(BlueprintCallable, Category = "Weapon")
	//����
	
	//�Զ�����
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void StartFire();
	//ֹͣ����
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void StopFire();
	UFUNCTION()
	void DestroyByUser(AUserCharacter* User);
	//��������
	void PlayMontage(UAnimInstance* AnimInstance, bool Play);
	//�������
	void Detach(FVector StartDirection, float Speed);
	//��ʾʰ����ʾ
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
		void ShowPickUpInfo(bool IsVisible);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		int ThisWeaponType;
};

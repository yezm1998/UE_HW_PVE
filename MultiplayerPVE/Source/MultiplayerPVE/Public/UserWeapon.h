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

//射击轨迹线信息
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
	//打击特效
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

	//服务器调用开火
	UFUNCTION(Server,Reliable,WithValidation)
	void ServerFire();
	//将设计路线传递到客服端
	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;
	UFUNCTION()
	void OnRep_HitScanTrace();

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	//动画相关
	UPROPERTY(EditDefaultsOnly, Category = "FireAnim")
	class UAnimationAsset* Anim;
	UPROPERTY(EditDefaultsOnly, Category = "FireAnim")
	class UAnimMontage* WeaponMontage;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	//UFUNCTION(BlueprintCallable, Category = "Weapon")
	//开镜
	
	//自动开火
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void StartFire();
	//停止开火
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void StopFire();
	UFUNCTION()
	void DestroyByUser(AUserCharacter* User);
	//武器动画
	void PlayMontage(UAnimInstance* AnimInstance, bool Play);
	//解除武器
	void Detach(FVector StartDirection, float Speed);
	//显示拾起提示
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
		void ShowPickUpInfo(bool IsVisible);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		int ThisWeaponType;
};

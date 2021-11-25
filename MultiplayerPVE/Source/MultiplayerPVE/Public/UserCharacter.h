// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UserCharacter.generated.h"
class UCameraComponent;
class USpringArmComponent;
class AUserWeapon;
class UUserHealthComponent;
class AUserWeaponGrenade;
//用户类;
UCLASS()
class MULTIPLAYERPVE_API AUserCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUserCharacter();
	
protected:
	// Called when the game starts or when spawned
	//用户名字
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		FString UserName = "111";


	virtual void BeginPlay() override;
	/*移动相关*/
	
	//前进
	void MoveForward(float Value);
	//右转
	void MoveRight(float Value);
	//蹲
	UFUNCTION()
	void BeginCrouch();
	//起
	UFUNCTION()
	void EndCrouch();



	/*武器相关*/
	//开火——连续开火
	//UFUNCTION()
		//void HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();
	void StartAim();
	void StopAim();
	void SartLookAround();
	void StopLookAround();
	
	//当前使用武器
	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		AUserWeapon* MyWeapon;
	//武器基类
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AUserWeapon> StarterWeaponClass[2];
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AUserWeaponGrenade> StarterThrowWeaponClass;
	//是否开火
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bFiring;
	//是否瞄准
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bAiming;
	//是否环视
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bLooking;
	//开镜
	bool bNeedToZoom;
	float ZoomFOV;
	float DefaultFOV;
	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadWrite, Category = "Components")//Replicated
		TArray<AUserWeapon*>  WeaponArr;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="Weapon")//
		int64 CurrentWeapon;
	TArray<FName>  WeaponSocket;
	//切换武器
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SwitchWeapon();
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void ServerSwitch();
	UFUNCTION(Server, Reliable)
		void ServerPickUp();
	void StartThrow();
	void EndThrow();
	AUserWeaponGrenade* Grenade;
	//播放武器蒙太奇动画
	void PlayAnimationByWeapon(bool Play);
	UPROPERTY(Replicated)
	AUserWeapon* NewWeapon;
	UFUNCTION(BlueprintImplementableEvent, Category = "SniperWeapon")
		void Sniper();
	UFUNCTION(BlueprintImplementableEvent, Category = "SniperWeapon")
		void UnSniper();

	/*摄像头*/
	UPROPERTY(VisibleAnywhere, Category = "CameraComponents")
		USpringArmComponent* SpringArmComp2;
	//第一人称镜头
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	//第三人称镜头
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="CameraComponents")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraComponents")
	USpringArmComponent* SpringArmComp;
	
	/*生命值*/
	//生命值
	/*UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		float Health;*/
	//float DefaultHealth;
	//是否死亡
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bDied;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUserHealthComponent* HealthComp;
	UFUNCTION()
	void OnHealthChanged(UUserHealthComponent* OwnerHealthComp, float Health, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	//重生
	void WaitForReborn();
	/*脚步效果*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
		UMaterialInterface* DefaultFloorEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire")
		UMaterialInterface* WaterFloorEffect;
	UFUNCTION(BlueprintCallable,Category="FootEffect")
		void WalkingEffect(FVector FootPosition);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable,Category="Health")
	float GetHealth();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FVector GetPawnViewLocation() const override;
	//装备新武器
	UFUNCTION()
	void EquippedWeapon(AUserWeapon* ANewWeapon,bool InOverlap);
	bool bEquip;
};

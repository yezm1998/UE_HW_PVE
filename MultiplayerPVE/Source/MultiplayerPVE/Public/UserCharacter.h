// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UserCharacter.generated.h"
class UCameraComponent;
class USpringArmComponent;
class AUserWeapon;
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
	virtual void BeginPlay() override;
	UFUNCTION()
		void HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
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
	//开火——连续开火
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();
	void StartAim();
	void StopAim();
	void SartLookAround();
	void StopLookAround();
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		TArray<AUserWeapon*>  WeaponArr;
	//当前使用武器
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		AUserWeapon*  MyWeapon;
	//武器基类
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AUserWeapon> StarterWeaponClass[2];
	//是否死亡
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bDied;
	//是否开火
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bFiring;
	//是否瞄准
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bAiming;
	//是否环视
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bLooking;
	//开镜
	bool bNeedToZoom;
	float ZoomFOV;
	float DefaultFOV;
	//生命值
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		float Health;
	float DefaultHealth;
	TArray<FName>  WeaponSocket;
	//用户名字
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	FString UserName="111";
	UFUNCTION(BlueprintCallable,Category="Weapon")
	void SwitchWeapon();
	int64 CurrentWeapon;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float GetHealth();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

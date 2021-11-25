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
//�û���;
UCLASS()
class MULTIPLAYERPVE_API AUserCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUserCharacter();
	
protected:
	// Called when the game starts or when spawned
	//�û�����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		FString UserName = "111";


	virtual void BeginPlay() override;
	/*�ƶ����*/
	
	//ǰ��
	void MoveForward(float Value);
	//��ת
	void MoveRight(float Value);
	//��
	UFUNCTION()
	void BeginCrouch();
	//��
	UFUNCTION()
	void EndCrouch();



	/*�������*/
	//���𡪡���������
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
	
	//��ǰʹ������
	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		AUserWeapon* MyWeapon;
	//��������
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AUserWeapon> StarterWeaponClass[2];
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AUserWeaponGrenade> StarterThrowWeaponClass;
	//�Ƿ񿪻�
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bFiring;
	//�Ƿ���׼
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bAiming;
	//�Ƿ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bLooking;
	//����
	bool bNeedToZoom;
	float ZoomFOV;
	float DefaultFOV;
	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadWrite, Category = "Components")//Replicated
		TArray<AUserWeapon*>  WeaponArr;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="Weapon")//
		int64 CurrentWeapon;
	TArray<FName>  WeaponSocket;
	//�л�����
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SwitchWeapon();
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void ServerSwitch();
	UFUNCTION(Server, Reliable)
		void ServerPickUp();
	void StartThrow();
	void EndThrow();
	AUserWeaponGrenade* Grenade;
	//����������̫�涯��
	void PlayAnimationByWeapon(bool Play);
	UPROPERTY(Replicated)
	AUserWeapon* NewWeapon;
	UFUNCTION(BlueprintImplementableEvent, Category = "SniperWeapon")
		void Sniper();
	UFUNCTION(BlueprintImplementableEvent, Category = "SniperWeapon")
		void UnSniper();

	/*����ͷ*/
	UPROPERTY(VisibleAnywhere, Category = "CameraComponents")
		USpringArmComponent* SpringArmComp2;
	//��һ�˳ƾ�ͷ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	//�����˳ƾ�ͷ
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="CameraComponents")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraComponents")
	USpringArmComponent* SpringArmComp;
	
	/*����ֵ*/
	//����ֵ
	/*UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		float Health;*/
	//float DefaultHealth;
	//�Ƿ�����
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bDied;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUserHealthComponent* HealthComp;
	UFUNCTION()
	void OnHealthChanged(UUserHealthComponent* OwnerHealthComp, float Health, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	//����
	void WaitForReborn();
	/*�Ų�Ч��*/
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
	//װ��������
	UFUNCTION()
	void EquippedWeapon(AUserWeapon* ANewWeapon,bool InOverlap);
	bool bEquip;
};

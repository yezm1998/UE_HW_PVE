// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UserCharacter.generated.h"
class UCameraComponent;
class USpringArmComponent;
class AUserWeapon;
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
	virtual void BeginPlay() override;
	UFUNCTION()
		void HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
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
	//���𡪡���������
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
	//��һ�˳ƾ�ͷ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	//�����˳ƾ�ͷ
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="CameraComponents")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraComponents")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		TArray<AUserWeapon*>  WeaponArr;
	//��ǰʹ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		AUserWeapon*  MyWeapon;
	//��������
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AUserWeapon> StarterWeaponClass[2];
	//�Ƿ�����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bDied;
	//�Ƿ񿪻�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bFiring;
	//�Ƿ���׼
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bAiming;
	//�Ƿ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bLooking;
	//����
	bool bNeedToZoom;
	float ZoomFOV;
	float DefaultFOV;
	//����ֵ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		float Health;
	float DefaultHealth;
	TArray<FName>  WeaponSocket;
	//�û�����
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

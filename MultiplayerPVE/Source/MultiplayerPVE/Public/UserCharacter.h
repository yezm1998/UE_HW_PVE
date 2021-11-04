// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UserCharacter.generated.h"
class UCameraComponent;
class USpringArmComponent;
class AUserWeapon;
//class USHealthComponent;
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
	//г╟╫Ь
	void MoveForward(float Value);
	//срв╙
	void MoveRight(float Value);
	UFUNCTION()
	void BeginCrouch();
	UFUNCTION()
	void EndCrouch();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="CameraComponents")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraComponents")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		AUserWeapon* MyWeapon;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AUserWeapon> StarterWeaponClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		bool bDied;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		float Health;
	UFUNCTION()
	void HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	float DefaultHealth = 100.0f;
	FName WeaponSocket;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	FString UserName="111";
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float GetHealth();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

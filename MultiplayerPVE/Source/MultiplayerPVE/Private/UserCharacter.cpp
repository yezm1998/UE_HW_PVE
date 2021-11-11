// Fill out your copyright notice in the Description page of Project Settings.


#include "UserCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UserWeapon.h"

// Sets default values
AUserCharacter::AUserCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bDied = false;
	Health = 100.0f;
	bFiring = false;
	bAiming = false;
	DefaultHealth = 100.0f;
	//CameraComp->bUsePawnControlRotation = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	SpringArmComp2 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp2"));
	SpringArmComp2->bUsePawnControlRotation = true; 
	SpringArmComp2->SetupAttachment(RootComponent);
	FollowCamera->SetupAttachment(SpringArmComp2);

	WeaponSocket.Add("HandSocket");
	WeaponSocket.Add("BackSocket");
	CurrentWeapon = 0;
	WeaponArr.Init(nullptr, 2);

	//HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	//SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	UE_LOG(LogTemp, Log, TEXT("BUild"));
}

// Called when the game starts or when spawned
void AUserCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	WeaponArr[0] = GetWorld()->SpawnActor<AUserWeapon>(StarterWeaponClass[0],FVector::ZeroVector,FRotator::ZeroRotator,SpawnParam);
	if (WeaponArr[0]) {
		WeaponArr[0]->SetOwner(this);
		WeaponArr[0]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket[0]);
	}
	WeaponArr[1] = GetWorld()->SpawnActor<AUserWeapon>(StarterWeaponClass[1], FVector::ZeroVector, FRotator::ZeroRotator, SpawnParam);
	if (WeaponArr[1]) {
		WeaponArr[1]->SetOwner(this);
		WeaponArr[1]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket[1]);
	}
	MyWeapon = WeaponArr[0];
	this->OnTakeAnyDamage.AddDynamic(this,&AUserCharacter::HandleDamage);
	DefaultFOV = CameraComp->FieldOfView;
	ZoomFOV = 60;
}

// ÒÆ¶¯
void AUserCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AUserCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AUserCharacter::BeginCrouch()
{
	Crouch();
}

void AUserCharacter::EndCrouch()
{
	UnCrouch();
}

void AUserCharacter::StartFire()
{
	
	MyWeapon->StartFire();
	//bFiring = true;
}

void AUserCharacter::StopFire()
{
	MyWeapon->StopFire();
	bFiring = false;
}

void AUserCharacter::StartAim()
{
	//UE_LOG(LogTemp, Log, TEXT("start"));
	bAiming = true;
	bNeedToZoom = true;
	//CameraComp->SetFieldOfView(ZoomFOV);
	FollowCamera->SetActive(true);
	CameraComp->SetActive(false);
	//this->bUseControllerRotationYaw = false;

}

void AUserCharacter::StopAim()
{
	bNeedToZoom = false;
	bAiming = false;
	//CameraComp->SetFieldOfView(DefaultFOV);
	FollowCamera->SetActive(false);
	CameraComp->SetActive(true);
	//this->bUseControllerRotationYaw = true;

}

void AUserCharacter::SartLookAround()
{
	//UE_LOG(LogTemp, Log, TEXT("start look around "));
	bLooking = true;
	this->bUseControllerRotationYaw = false;
	//this->bUseControllerRotationPitch = false;
}

void AUserCharacter::StopLookAround()
{
	//UE_LOG(LogTemp, Log, TEXT("stop look around "));
	bLooking = false;
	this->bUseControllerRotationYaw = true;
	//this->bUseControllerRotationPitch = true;

}


void AUserCharacter::SwitchWeapon()
{
	if (!CurrentWeapon) {
		WeaponArr[1]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket[0]);
		WeaponArr[0]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket[1]);
	}
	else {
		WeaponArr[1]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket[1]);
		WeaponArr[0]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket[0]);
	}
	//PlaySwitchAnimation();
	CurrentWeapon = 1 - CurrentWeapon;
	MyWeapon = WeaponArr[CurrentWeapon];
	
}

// Called every frame
void AUserCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

float AUserCharacter::GetHealth()
{
	return Health;
}

void AUserCharacter::HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
	
	if (Health<=0 || Damage <= 0) {
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	UE_LOG(LogTemp, Log, TEXT("%f %f"),Health,Damage);
	if (Health <= 0) {
		bDied = true;
		GetMovementComponent()->StopMovementImmediately();
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		SetLifeSpan(5.0f);
	}
		
}

// Called to bind functionality to input
void AUserCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AUserCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUserCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AUserCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AUserCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AUserCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("UnCrouch", IE_Released, this, &AUserCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AUserCharacter::Jump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed,this, &AUserCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AUserCharacter::StopFire);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AUserCharacter::StartAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AUserCharacter::StopAim);
	PlayerInputComponent->BindAction("FreePerspective", IE_Pressed, this, &AUserCharacter::SartLookAround);
	PlayerInputComponent->BindAction("FreePerspective", IE_Released, this, &AUserCharacter::StopLookAround);
	//PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AUserCharacter::SwitchWeapon);
	//PlayerInputComponent->BindAction("SwitchWeapon", IE_Released, this, &AUserCharacter::StopLookAround);
}


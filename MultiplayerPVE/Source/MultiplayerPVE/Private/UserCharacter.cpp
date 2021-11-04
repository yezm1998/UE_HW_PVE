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
	Health = 100;
	//CameraComp->bUsePawnControlRotation = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	WeaponSocket = "WeaponSocket";
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
	MyWeapon = GetWorld()->SpawnActor<AUserWeapon>(StarterWeaponClass,FVector::ZeroVector,FRotator::ZeroRotator,SpawnParam);
	if (MyWeapon) {
		MyWeapon->SetOwner(this);
		MyWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
	this->OnTakeAnyDamage.AddDynamic(this,&AUserCharacter::HandleDamage);
	//HealthComponent->Add
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

void AUserCharacter::Fire()
{
	MyWeapon->Fire();
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
	//if(Weapon)
	PlayerInputComponent->BindAction("Fire", IE_Pressed,this, &AUserCharacter::Fire);
}


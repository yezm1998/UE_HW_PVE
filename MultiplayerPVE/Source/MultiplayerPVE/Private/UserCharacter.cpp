// Fill out your copyright notice in the Description page of Project Settings.


#include "UserCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UserWeapon.h"
#include "PVEGameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UserPlayerState.h"
#include "UserWeaponGrenade.h"
#include "../Components/UserHealthComponent.h"
// Sets default values
AUserCharacter::AUserCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bDied = false;
	
	bFiring = false;
	bAiming = false;
	bEquip = false;
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
	WeaponSocket.Add("ThrowGenade");
	WeaponArr.Init(nullptr, 2);

	HealthComp = CreateDefaultSubobject<UUserHealthComponent>("HealthComp");
	HealthComp->SetNetAddressable();
	HealthComp->SetIsReplicated(true);
	//Health = HealthComp->GetHealth();
	//HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	//SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	UE_LOG(LogTemp, Log, TEXT("BUild"));
}

// Called when the game starts or when spawned
void AUserCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComp->OnHealthChanged.AddDynamic(this, &AUserCharacter::OnHealthChanged);
	DefaultFOV = CameraComp->FieldOfView;
	ZoomFOV = 60;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	if (GetLocalRole() == ROLE_Authority) {
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		WeaponArr[0] = GetWorld()->SpawnActor<AUserWeapon>(StarterWeaponClass[0], FVector::ZeroVector, FRotator::ZeroRotator, SpawnParam);
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
		CurrentWeapon = 0;
	}
	
	//this->OnTakeAnyDamage.AddDynamic(this,&AUserCharacter::HandleDamage);
	
}

// 移动
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
	//if (GetLocalRole() == ROLE_Authority) {
	if (MyWeapon) {
		MyWeapon->StartFire();
		bFiring = true;
		PlayAnimationByWeapon(true);
	}
			
	//}
	
}

void AUserCharacter::StopFire()
{
	//if (GetLocalRole() == ROLE_Authority) {
	if (MyWeapon) {
		MyWeapon->StopFire();
		bFiring = false;
		PlayAnimationByWeapon(false);
	}
			
	//}
}

void AUserCharacter::StartAim()
{
	//UE_LOG(LogTemp, Log, TEXT("start"));
	//if (GetLocalRole() == ROLE_Authority) 
	bAiming = true;

	bNeedToZoom = true;
	FollowCamera->SetActive(true);
	if (MyWeapon->ThisWeaponType == 1) {
		FollowCamera->SetFieldOfView(90);
	}
	if (MyWeapon->ThisWeaponType == 2) {
		FollowCamera->SetFieldOfView(30);
		Sniper();
	}
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
	if (MyWeapon->ThisWeaponType == 2) {
		UnSniper();
	}
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
	if (GetLocalRole() != ROLE_Authority) {
		//ServerSwitch();
		return;
	}
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

void AUserCharacter::ServerSwitch_Implementation()
{
	//SwitchWeapon();
	/*if (GetLocalRole() == ROLE_AutonomousProxy) {
		return;
	}*/
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
bool AUserCharacter::ServerSwitch_Validate()
{
	return true;
}


void AUserCharacter::ServerPickUp_Implementation()
{
	if (NewWeapon) {
		WeaponArr[CurrentWeapon]->SetOwner(nullptr);
		WeaponArr[CurrentWeapon]->Detach(UKismetMathLibrary::GetForwardVector(GetControlRotation()), 100);
		//WeaponArr[CurrentWeapon]->AddImpulse(AddImpulse); (this);
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket[0]);
		NewWeapon->SetOwner(this);
		
		WeaponArr[CurrentWeapon] = NewWeapon;
		MyWeapon = WeaponArr[CurrentWeapon];
		NewWeapon = nullptr;
	}
	//StartThrow();
}

void AUserCharacter::StartThrow()
{
	//bEquip = false;
	//WeaponArr[CurrentWeapon]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (GetLocalRole() != ROLE_Authority) {
		ServerPickUp();
		//return;
	}
	NewWeapon->ShowPickUpInfo(false);
	
}





void AUserCharacter::PlayAnimationByWeapon(bool Play)
{
	MyWeapon->PlayMontage(GetMesh()->GetAnimInstance(), Play);
}

void AUserCharacter::EquippedWeapon(AUserWeapon* ANewWeapon,bool InOverlap)
{
	if (InOverlap) {
		this->NewWeapon = ANewWeapon;
	}
	else {
		if (this->NewWeapon == ANewWeapon)
			this->NewWeapon = nullptr;
	}
	
}


void AUserCharacter::OnHealthChanged(UUserHealthComponent* OwnerHealthComp, float Health, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0 && !bDied) {
		AUserPlayerState* PS=Cast<AUserPlayerState>(GetPlayerState());
		PS->AddDiedNumber(1);
		bDied = true;
		GetMovementComponent()->StopMovementImmediately();
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponArr[0]->DestroyByUser(this);
		WeaponArr[1]->DestroyByUser(this);
		//SetLifeSpan(2.0f);

		if (GetLocalRole() == ROLE_Authority)
		{
				GetWorldTimerManager().SetTimer(TimerHandle_LifeSpanExpired, this, &AUserCharacter::WaitForReborn, 5, false);
		}
		
		APVEGameModeBase* GM = Cast<APVEGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GM) {
			if(InstigatedBy)
				GM->OnActorKilled.Broadcast(this, this->GetInstigator(), DamageCauser, InstigatedBy);
			else
				GM->OnActorKilled.Broadcast(this, this->GetInstigator(), DamageCauser, DamageCauser->GetOwner());
		}
	}
}
void AUserCharacter::WaitForReborn() {
	DetachFromControllerPendingDestroy();
	Destroy();
}

//行走脚步贴图和声效
void AUserCharacter::WalkingEffect(FVector FootPosition)
{
	//FootPosition 脚步位置
	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bReturnPhysicalMaterial = true;
	FVector EndPosition = FootPosition + FVector::UpVector * -100;
	if (GetWorld()->LineTraceSingleByChannel(Hit, FootPosition, EndPosition, ECC_Visibility, QueryParams)) {
		UMaterialInterface* SelectedEffect = nullptr; 
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		switch (SurfaceType)
		{
		case FloorDefault:
			UE_LOG(LogTemp, Log, TEXT("floot_default"));
			SelectedEffect = DefaultFloorEffect;
			break;
		case WaterDefault:
			UE_LOG(LogTemp, Log, TEXT("floot_water"));
			SelectedEffect = WaterFloorEffect;
			break;
		default:
			SelectedEffect = DefaultFloorEffect;
			break;
		}
		if (SelectedEffect) {
			UGameplayStatics::SpawnDecalAtLocation(GetWorld(), SelectedEffect, { 10,20,10 }, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), 5);
		}
	}
}

// Called every frame
void AUserCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}



float AUserCharacter::GetHealth()
{
	return HealthComp->GetHealth();
}
/*
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

		APVEGameModeBase* GM = Cast<APVEGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GM) {
			GM->OnActorKilled.Broadcast(this, DamageCauser,InstigatedBy );
		}
	}
    
		
}
*/
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
	PlayerInputComponent->BindAction("PickupWeapon", IE_Pressed, this, &AUserCharacter::StartThrow);
	//PlayerInputComponent->BindAction("PickupWeapon", IE_Released, this, &AUserCharacter::EndThrow);
}

FVector AUserCharacter::GetPawnViewLocation() const
{
	if (CameraComp->IsActive()) {
		return CameraComp->GetComponentLocation();
	}
	else if (FollowCamera->IsActive()) {
		return FollowCamera->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void AUserCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUserCharacter, MyWeapon);
	DOREPLIFETIME(AUserCharacter, NewWeapon);
	DOREPLIFETIME(AUserCharacter, WeaponArr);
	DOREPLIFETIME(AUserCharacter, CurrentWeapon);
	DOREPLIFETIME(AUserCharacter, bDied);
	DOREPLIFETIME(AUserCharacter, bFiring);
	DOREPLIFETIME(AUserCharacter, bAiming);
	//DOREPLIFETIME(AUserCharacter, HealthComp);
}
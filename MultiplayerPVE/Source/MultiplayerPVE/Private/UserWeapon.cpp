// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Net/UnrealNetwork.h"
#include "UserCharacter.h"
#include "Camera\CameraShake.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimationAsset.h"
// Sets default values
AUserWeapon::AUserWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MuzzlePosition = CreateDefaultSubobject<USphereComponent>(TEXT("MuzzlePosition"));
	MuzzlePosition->SetupAttachment(MeshComponent);
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(MeshComponent);
	SphereComp->SetSphereRadius(50);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	MeshComponent->SetCollisionObjectType(ECC_PhysicsBody);

	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ShootRange = 10000.0;
	BasicDamage = 25;
	RateOfFire = 300;
	MuzzleSocketName = TEXT("MuzzleSocket");
	ThisWeaponType = WeaponType::Dagger;
	SetReplicates(true);
	NetUpdateFrequency = 66;
	MinNetUpdateFrequency = 33;
	HitScanTrace.FireTime = 0;
}

// Called when the game starts or when spawned
void AUserWeapon::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AUserWeapon::OnOverlapBegin);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AUserWeapon::OnOverlapEnd);
	if (Anim && GetOwner()) {
		Cast<AUserCharacter>(GetOwner())->GetMesh()->PlayAnimation(Anim,false);
	}
}

void AUserWeapon::PlayEffect(FVector TraceEndPoint)
{
	if (MuzzleEffect) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
	}
	APawn* MyOwener = Cast<APawn>(GetOwner());
	if (MyOwener) {
		APlayerController* PC = Cast<APlayerController>(MyOwener->GetController());
		if (PC) {
			PC->ClientPlayCameraShake(FireShake);
		}
	}
	if (!AudioComp->IsPlaying()) {
		AudioComp->Play();
	}
	if (TraceEffect) {
		FVector MuzzleLoction = MeshComponent->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, MuzzleLoction);
		if (TracerComp) {
			TracerComp->SetVectorParameter("BeamEnd", TraceEndPoint);
		}
	}
}

void AUserWeapon::PlayImpactEffect(EPhysicalSurface SurfaceType, FHitResult Hit)
{
	UParticleSystem* SelectedEffect = nullptr;
	switch (SurfaceType)
	{
	case SurfaceDefault:
		UE_LOG(LogTemp, Log, TEXT("default"));
		SelectedEffect = DefaultHitEffect;
		break;
	case SurfaceFlesh:
		UE_LOG(LogTemp, Log, TEXT("blood"));
		BasicDamage *= 2;
		SelectedEffect = FleshBloodHitEffect;
		break;
	default:
		SelectedEffect = DefaultHitEffect;
		break;
	}
	if (SelectedEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	}
}

void AUserWeapon::Fire()
{
	if (GetLocalRole() != ROLE_Authority) {
		ServerFire();
	}
	/*if (ThisWeaponType == WeaponType::Dagger) {
		DaggerFire();
	}
	else if (ThisWeaponType == WeaponType::Gun) {*/
		AActor* MyPawn = GetOwner();
		if (MyPawn) {
			FHitResult Hit;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(MyPawn);
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;
			QueryParams.bReturnPhysicalMaterial = true;
			//枪口位置
			FVector Rotation = MuzzlePosition->GetForwardVector();
			//视线位置保证射击屏幕中央
			FVector EyePosition; FRotator EyeRotation;
			MyPawn->GetActorEyesViewPoint(EyePosition, EyeRotation);
			//枪口前方  FVector EndPosition = MuzzlePosition->GetComponentLocation() + Rotation * ShootRange;
			//视野前方
			FVector EndPosition = EyePosition + EyeRotation.Vector() * ShootRange;
			EPhysicalSurface SurfaceType = SurfaceType_Default;
			FVector TraceEndPoint = EndPosition;
			if (GetWorld()->LineTraceSingleByChannel(Hit, MuzzlePosition->GetComponentLocation(), EndPosition, CollisionWeapon, QueryParams)) {
				AActor* HitActor = Hit.GetActor();

				//获取目标材质
				SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

				PlayImpactEffect(SurfaceType, Hit);
				//造成伤害
				UGameplayStatics::ApplyPointDamage(HitActor, BasicDamage, Rotation, Hit, MyPawn->GetInstigatorController(), this, DamageType);

				TraceEndPoint = Hit.ImpactPoint;
			}
			//DrawDebugLine(GetWorld(), MuzzlePosition->GetComponentLocation(), EndPosition, FColor::White, false, 0.2f, 0, 0.2f);

			if (GetLocalRole() == ROLE_Authority) {
				HitScanTrace.TraceEnd = TraceEndPoint;
				HitScanTrace.SurfaceType = SurfaceType;
				HitScanTrace.HitResult = Hit;
				HitScanTrace.FireTime++;
			}
			PlayEffect(TraceEndPoint);
			//Cast<AUserCharacter>(MyPawn)->PlayAnimationByWeapon();
		}
	
	//GetWorld()->TimeSeconds();
}

void AUserWeapon::OnRep_HitScanTrace()
{
	PlayEffect(HitScanTrace.TraceEnd);
	PlayImpactEffect(HitScanTrace.SurfaceType, HitScanTrace.HitResult);
}

void AUserWeapon::ServerFire_Implementation()
{
	Fire();
}

bool AUserWeapon::ServerFire_Validate()
{
	return true;
}

void AUserWeapon::StartFire()
{
	//防止点击过快超过射速
	//float FirstDelay = FMath::Max (LastTime + RateOfFire - GetWorld()->TimeSeconds , 0.0f);
	GetWorldTimerManager().SetTimer(FireTimer,this,&AUserWeapon::Fire, 60/ RateOfFire, true, 0);

}

void AUserWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
}

// Called every frame
void AUserWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUserWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AUserWeapon, HitScanTrace,COND_SkipOwner);
}

void AUserWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner())
		return;
	ShowPickUpInfo(true);
	UE_LOG(LogTemp, Warning, TEXT("Weapon:OverLap"));
	AUserCharacter* User = Cast<AUserCharacter>(OtherActor);
	if (User) {
		User->EquippedWeapon(this,true);
		MeshComponent->SetSimulatePhysics(false);
	}
}

void AUserWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetOwner())
		return;
	ShowPickUpInfo(false);
	UE_LOG(LogTemp, Warning, TEXT("Weapon:OverLapEnd"));
	AUserCharacter* User = Cast<AUserCharacter>(OtherActor);
	if (User) {
		User->EquippedWeapon(this,false);
		MeshComponent->SetSimulatePhysics(true);
	}
}
void AUserWeapon::DestroyByUser(AUserCharacter* User) {
	if(User==Cast<AUserCharacter>(GetOwner()))
	{
		Destroy();
	}
	
}

void AUserWeapon::PlayMontage(UAnimInstance* AnimInstance,bool Play)
{
	if (AnimInstance) {
		if (WeaponMontage) {
			if (Play)
				AnimInstance->Montage_Play(WeaponMontage);
			else
				AnimInstance->Montage_Stop(0.1,WeaponMontage);
		}
	}
}

void AUserWeapon::Detach(FVector StartDirection,float Speed)
{
	MeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetEnableGravity(true);
	MeshComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	//UE_LOG(LogTemp, Log, TEXT("LOCATION:%d,%d,%d"), StartDirection.X, StartDirection.Y, StartDirection.Z );
	MeshComponent->AddImpulse(StartDirection * Speed);
}

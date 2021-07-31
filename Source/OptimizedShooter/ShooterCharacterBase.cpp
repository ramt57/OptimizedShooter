// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacterBase.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Plane.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AShooterCharacterBase::AShooterCharacterBase(): BaseTurnRate(45.f), BaseLookUpRate(45.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Spring Arm for Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = SpringArmLength;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 40.f, 50.f);
	//Follow actual camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; //does not rotate on its own but will the spring arm  Camera Boom

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 440.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = .2f;
}

// Called when the game starts or when spawned
void AShooterCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("C++ Character is spawned in the world"));
}

// Called every frame
void AShooterCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent) //check macro to validate the passing component if its set or non null

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacterBase::TurnRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacterBase::LookUpRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AShooterCharacterBase::FireWeapon);
}

void AShooterCharacterBase::SetCameraBoomZoom(float value)
{
	CameraBoom->TargetArmLength = value * SpringArmLength;
}

void AShooterCharacterBase::MoveForward(const float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		//find out the forward direction with the rotation matrix
		const FRotator Rotation{Controller->GetControlRotation()};
		const FRotator YawRotation{0, Rotation.Yaw, 0};

		//Basically a forward vector toward the direction of looking.
		const FVector Direction{FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X)};
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacterBase::MoveRight(const float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		//find out the forward direction with the rotation matrix
		const FRotator Rotation{Controller->GetControlRotation()};
		const FRotator YawRotation{0, Rotation.Yaw, 0};

		//Basically a forward vector toward the direction of looking.
		const FVector Direction{FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y)};
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacterBase::TurnRate(const float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacterBase::LookUpRate(const float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AShooterCharacterBase::FireWeapon()
{
	if (FireSound != nullptr)
	{
		//UGameplayStatics::PlaySound2D(this, FireSound);
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireMontage)
	{
		AnimInstance->Montage_Play(FireMontage);
	}
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("MuzzleFlash");
	FTransform SocketTransform;
	if (BarrelSocket)
	{
		SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}
	}
	
	FVector BeamEndPoint;
	if(GetBeamEndLocation(SocketTransform.GetLocation(), BeamEndPoint))
	{
		if (ImpactParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, BeamEndPoint);
		}

		if (BeamParticle)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(), BeamParticle, SocketTransform);
			
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEndPoint);
			}
		}
	}
}

bool AShooterCharacterBase::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation) const
{
	FVector2D ViewPortSize;
	GetViewPortSize(ViewPortSize);

	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	FVector2D CrosshairLocation(ViewPortSize.X / 2, ViewPortSize.Y / 2);
	CrosshairLocation.Y -= 50.f;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection
	);

	if (bScreenToWorld)
	{
		FHitResult FCrosshairHitResult;
		const FVector CSHStart{CrosshairWorldPosition};
		const FVector CSHEnd{CSHStart + CrosshairWorldDirection * 50'000.f};

		OutBeamLocation = CSHEnd;

		/*Trace Outward from crosshair wrld location*/
		GetWorld()->LineTraceSingleByChannel(FCrosshairHitResult, CSHStart, CSHEnd, ECC_Visibility);
		if (FCrosshairHitResult.bBlockingHit)
		{
			OutBeamLocation = FCrosshairHitResult.Location;
		}

		FHitResult FWeaponHitResult;
		const FVector WPStart{MuzzleSocketLocation};
		const FVector WPEnd{OutBeamLocation};

		/*Trace outword from weapon barrel to world*/
		GetWorld()->LineTraceSingleByChannel(FWeaponHitResult, WPStart, WPEnd, ECC_Visibility);
		if (FWeaponHitResult.bBlockingHit)
		{
			OutBeamLocation = FWeaponHitResult.Location;
		}
		return true;
	}
	return false;
}

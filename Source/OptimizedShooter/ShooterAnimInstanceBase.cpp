// Fill out your copyright notice in the Description page of Project Settings.
#include "ShooterAnimInstanceBase.h"
#include "ShooterCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UShooterAnimInstanceBase::NativeInitializeAnimation()
{
	if (ShooterCharacterBase == nullptr) {
		ShooterCharacterBase = Cast<AShooterCharacterBase>(TryGetPawnOwner());
	}
}
void UShooterAnimInstanceBase::UpdateAnimationProperties(float DeltaTime) {
	if (ShooterCharacterBase == nullptr) {
		ShooterCharacterBase = Cast<AShooterCharacterBase>(TryGetPawnOwner());
	}
	if (ShooterCharacterBase != nullptr) {
		FVector Velocity{ ShooterCharacterBase->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();
		bIsInAir = ShooterCharacterBase->GetCharacterMovement()->IsFalling();
		if (ShooterCharacterBase->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f) {
			bIsAccelerating = true;
		}
		else {
			bIsAccelerating = false;
		}
		/* Calculating the movement offset for strafing */
		FRotator AimRotation=ShooterCharacterBase->GetBaseAimRotation();
		FRotator MovementRotationFromX=UKismetMathLibrary::MakeRotFromX(ShooterCharacterBase->GetVelocity());
		MovementOffset=UKismetMathLibrary::NormalizedDeltaRotator(MovementRotationFromX,AimRotation).Yaw;
		
	}
	else {
		UE_LOG(LogTemp,Warning,TEXT("Shooter character is null, this should not happen here"))
	}
}
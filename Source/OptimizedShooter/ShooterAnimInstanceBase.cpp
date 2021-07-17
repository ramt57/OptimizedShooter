// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstanceBase.h"
#include "ShooterCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

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
		speed = Velocity.Size();
		isInAir = ShooterCharacterBase->GetCharacterMovement()->IsFalling();
		if (ShooterCharacterBase->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f) {
			isAccelerating = true;
		}
		else {
			isAccelerating = false;
		}
	}
	else {
		UE_LOG(LogTemp,Warning,TEXT("Shooter character is null, this should not happen here"))
	}
}
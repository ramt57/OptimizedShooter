// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterWeaponBase.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void AShooterWeaponBase::BeginPlay()
{

}

void AShooterWeaponBase::FireWeapon() const
{
	if(FireSound!=nullptr)
	{
		UGameplayStatics::PlaySound2D(this,FireSound);
	}
	//Playing AnimMontage for weapon Fire
	// UAnimInstance* AnimInstance=GetComponentByClass<USkeletalMeshComponent>(Text)
}



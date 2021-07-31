// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterItemBase.h"
#include "ShooterWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class OPTIMIZEDSHOOTER_API AShooterWeaponBase : public AShooterItemBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UAnimInstance* FireMontage;
	
public:
	virtual void BeginPlay() override;
	void FireWeapon() const;
};

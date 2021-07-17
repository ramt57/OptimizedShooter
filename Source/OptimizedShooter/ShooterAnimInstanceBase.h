// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class OPTIMIZEDSHOOTER_API UShooterAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Movement",meta=(AllowPrivateAccess="true"))
	class AShooterCharacterBase* ShooterCharacterBase;

	//Character lateral speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float speed;
	//whether character is in air
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool isInAir;
	//whether character is accelerating
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool isAccelerating;
};

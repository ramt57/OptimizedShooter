// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacterBase.generated.h"

UCLASS()
class OPTIMIZEDSHOOTER_API AShooterCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* BeamParticle;
public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable, Category = "CameraZoom")
	void SetCameraBoomZoom(float value);

	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arm Length")
	float SpringArmLength = 300.f;

	//Movement control function
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float Value); //moving character forward & backward
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveRight(float Value); //moving character left & right

	//Mouse control functions
	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
	void TurnRate(float Rate);
	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
	void LookUpRate(float Rate);

	//Firing related logic needs refactoring in future as the weapon logic should be in the weapon class not in the character class
	void FireWeapon();
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation,FVector& OutBeamLocation) const;
	
	FORCEINLINE static void GetViewPortSize(FVector2D& ViewPortSize)
	{
		if(GEngine&&GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewPortSize);
		}
	}
};

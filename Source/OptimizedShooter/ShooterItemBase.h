// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterItemBase.generated.h"

UCLASS()
class OPTIMIZEDSHOOTER_API AShooterItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Item Properties" , meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* ItemMesh;
public:
};

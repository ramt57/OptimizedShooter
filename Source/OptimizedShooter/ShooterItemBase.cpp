// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterItemBase.h"
#include "Components/BoxComponent.h"
// Sets default values
AShooterItemBase::AShooterItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(ItemMesh);
}

// Called when the game starts or when spawned
void AShooterItemBase::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AShooterItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


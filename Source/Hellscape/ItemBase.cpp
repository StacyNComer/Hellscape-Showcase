// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemBase.h"
#include "Hellscape.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(CPROFILE_PICKUP);
	Mesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::SetDropState(bool isDropped)
{
	Mesh->SetSimulatePhysics(isDropped);
	Mesh->SetCollisionEnabled(isDropped ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	Mesh->SetVisibility(isDropped);
}


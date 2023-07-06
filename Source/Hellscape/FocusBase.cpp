// Fill out your copyright notice in the Description page of Project Settings.

#include "FocusBase.h"

AFocusBase::AFocusBase()
{
	itemType = EItemType::Focus;
	itemTypeName = NSLOCTEXT("ItemTypeName", "FocusType", "Focus");

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void AFocusBase::SetWielded(bool isWielded)
{
	Mesh->SetVisibility(isWielded);
}

void AFocusBase::StartCasting_Implementation()
{
	isCasting = true;
}

void AFocusBase::StopCasting_Implementation()
{
	isCasting = false;
}
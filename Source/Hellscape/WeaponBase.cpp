// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "PlayerCharacterBase.h"

AWeaponBase::AWeaponBase()
{
	cooldownTracker = 0;
	castDelayTracker = 0;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//GetDefaultObject does not work in constructors :(
	attackDamageType = attackDamageTypeClass.GetDefaultObject();
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (cooldownTracker > 0)
		cooldownTracker -= DeltaTime;

	if (castDelayTracker > 0)
		castDelayTracker -= DeltaTime;
}

void AWeaponBase::SetDropState(bool isDropped)
{
	Super::SetDropState(isDropped);

	isCasting = false;
}

void AWeaponBase::StartCasting_Implementation()
{

}

void AWeaponBase::StopCasting_Implementation()
{

}

bool AWeaponBase::CustomCastCheck_Implementation()
{
	return true;
}

//TODO (eventually): Add softmana cost.
void AWeaponBase::Cast_Implementation()
{
	
}

void AWeaponBase::AttemptCast()
{
	if (castDelayTracker > 0)
	{
	}
	else if (cooldownTracker > 0)
	{
		if (NoCooldownForceCast.IsBound())
			NoCooldownForceCast.Broadcast();
		else
			NoCooldownCastFailed.Broadcast();
	}
	else if (wieldingPlayer->currentMana < manaCost + softManaCost)
	{
		if (NoManaForceCast.IsBound())
			NoManaForceCast.Broadcast();
		else
			NoManaCastFailed.Broadcast();
	}
	else
	{
		//Expend Resources
		wieldingPlayer->currentMana -= manaCost.modified;
		cooldownTracker = cooldown;
		castDelayTracker = castDelay;

		Cast();
	}
}

void AWeaponBase::UpdateStatModifier(FMultipliedStat& stat)
{
	stat.modified = stat.base;
}

#if WITH_EDITOR
void AWeaponBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UpdateStatModifier(damage);

	UpdateStatModifier(potency);

	UpdateStatModifier(critDamageMultiplier);

	UpdateStatModifier(critPotencyMultiplier);

	UpdateStatModifier(manaCost);

	UpdateStatModifier(softManaCost);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "FocusBase.generated.h"

/**
 * 
 */
UCLASS()
class HELLSCAPE_API AFocusBase : public AWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		/*
		The transform weapons are parented to when equipped.
		*/
		class USceneComponent* ProjectileSpawnPoint;

public:
	AFocusBase();

	/*
	Toggles the visibility of the wand's mesh and any other effects that should appear when the wand is wielded
	*/
	virtual void SetWielded(bool isWielded);
};

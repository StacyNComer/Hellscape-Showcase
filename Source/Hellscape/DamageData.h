// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageData.generated.h"

class APlayerCharacterBase;
class AWeaponBase;

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		APlayerCharacterBase* attacker;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AWeaponBase* damageSource;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool iaAOE;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool criticalHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool triggerDamageTraits;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class HELLSCAPE_API UDamageData : public UObject
{
	 GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float potency;

	UFUNCTION(Blueprintcallable)
	static UDamageData* CreateDamageData(float attackDamage, float attackPotency)
	{
		auto instance = NewObject<UDamageData>();
		instance->damage = attackDamage;
		instance->potency = attackPotency;

		return instance;
	}
};

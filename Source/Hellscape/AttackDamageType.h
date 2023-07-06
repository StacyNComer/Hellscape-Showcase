// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttackDamageType.generated.h"

UENUM(BlueprintType)
/*
* The type of damage type an attack deals and kind of status effect buildup it applies.
* If a new damage type is added, the buildup for the relevant type is automatically added to players and enemies.
*/
enum class EDamageType : uint8
{
	Physical	UMETA(DisplayName = "Physical"),
	Fire		UMETA(DisplayName = "Fire"),
	Water		UMETA(DisplayName = "Water"),
	Hex			UMETA(DisplayName = "Hex"),
};

UENUM(BlueprintType)
enum class EStatusEffect : uint8
{
	Stagger		UMETA(DisplayName = "Stagger"),
	Burn		UMETA(DisplayName = "Burning"),
	Irradiate	UMETA(DisplayName = "Irradiate"),
	Freeze		UMETA(DisplayName = "Freezing"),
	Shock		UMETA(DisplayName = "Shocking"),
	Seal		UMETA(DisplayName = "Sealing")
};

UCLASS(Blueprintable, BlueprintType)
/**
 * Links damage types to the status effects they cause.
 */
class HELLSCAPE_API UAttackDamageType : public UObject
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Damage Data")
		static FText GetStatusEffectDescription(EStatusEffect statusEffect)
		{
			switch (statusEffect)
			{
			case EStatusEffect::Stagger:
				return NSLOCTEXT("AttackDamageType", "StaggerDesc", "Interrupts a Hellspawn's attacks and movement for a brief moment.");
			case EStatusEffect::Burn:
				return NSLOCTEXT("AttackDamageType", "BurnDesc", "The afflicted Hellspawn takes fire damage for the next 8 seconds.");
			case EStatusEffect::Irradiate:
				return NSLOCTEXT("AttackDamageType", "IrradiateDesc", "Slows a Hellspawn's movement and animations while reducing the damage they deal for 9 seconds.");
			case EStatusEffect::Freeze:
				return NSLOCTEXT("AttackDamageType", "FreezeDesc", "Prevents a Hellspawn from moving or attacking. Damaging the Hellspawn with a Spell or Foci greatly shortens the effect.");
			case EStatusEffect::Shock:
				return NSLOCTEXT("AttackDamageType", "ShockDesc", "The Hellspawn instantly takes Water damage equal to a percent of their max health.");
			case EStatusEffect::Seal:
				return NSLOCTEXT("AttackDamageType", "Seal", "Nullifies an afflicted Hellspawn's traits.");
			default:
				return FText::FromString("I am a naughty status effect with an undefined description >:). My ID is: " + (uint8)statusEffect);

			}
		}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EDamageType damageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EStatusEffect statusEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		//Controls the color of weapon effects using this Attack Type.
		FLinearColor color;
};

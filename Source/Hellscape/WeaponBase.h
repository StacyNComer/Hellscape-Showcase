// Fill out your copyright notice in the Description page of Project Settings.

//NOTE: Weapon functions may not reference the properties or methods of PlayerCharacterBase, unless you know how to handle dependecies better than I do.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "AttackDamageType.h"
#include "WeaponBase.generated.h"

class APlayerCharacterBase;

USTRUCT(BlueprintType, Category = "Game Stats")
// Used for stats that are ran by a multiplier. The modified value updates automatically.
struct FMultipliedStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float base = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float multiplier = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float modified = 0;

	float operator + (FMultipliedStat const& stat)
	{
		return modified + stat.modified;
	}
};

UCLASS()
/**
 * The base class for weapons, which include the game's Spells and Foci.
 */
class HELLSCAPE_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:
	#pragma region UDelegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponForceCast);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon Casting")
		/*
		* Casts the spell/focus. The checks for mana/cooldown/etc are ignored in this method. AttemptCast should be used if these checks are needed.
		*/
		void Cast();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon Casting")
		/*Override to add additional requirements for casting the spell. Simply returns true if not overriden.*/
		bool CustomCastCheck();

	UPROPERTY(BlueprintAssignable, Category = "Weapon Casting")
		/*
		* Alternate behavior for if the weapon is cast without enough mana. If this is unbound, NoManaCastFailed should be called.
		*/
		FWeaponForceCast NoManaForceCast;

	UPROPERTY(BlueprintAssignable, Category = "Weapon Casting")
		/*
		* Alternate behavior for if the weapon is cast while on cooldown. If this is unbound, NoCooldownCastFailed should be called.
		*/
		FWeaponForceCast NoCooldownForceCast;

	UPROPERTY(BlueprintAssignable, Category = "Weapon Casting")
		/*
		* Fires when the weapon is cast with insufficient mana. If a weapon has alt behavior for being cast with no mana, NoManaForceCast may be called instead.
		*/
		FWeaponForceCast NoManaCastFailed;

	UPROPERTY(BlueprintAssignable, Category = "Weapon Casting")
		/*
		* Fires when the weapon is cast on cooldown. If the weapon has alt behavior for being cast on cooldown, NoCooldownForceCast may be called instead.
		*/
		FWeaponForceCast NoCooldownCastFailed;
	#pragma endregion

	#pragma region UPROPERTIES
	//Weapon Stats
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Weapon Stats")
		//The class representing this weapon's damage type and status effect. Use the weapon's attackDamageType to access the classes properties.
		TSubclassOf<class UAttackDamageType> attackDamageTypeClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Weapon Stats")
		UAttackDamageType* attackDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Stats")
		FMultipliedStat damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Stats")
		FMultipliedStat potency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Stats")
		FMultipliedStat critDamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Stats")
		FMultipliedStat critPotencyMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Stats")
		float castDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Stats")
		float cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Stats")
		FMultipliedStat manaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Stats")
		FMultipliedStat softManaCost;

	/*
	* Makes the behavior of the weapon's area of effect more intense. The weapon has no area of effect and is strictly single targe at 0.
	* This has different effects on different weapons: For example, it may increase a weapon's explosive radius or cause it to pierce an additional enemy.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Stats")
		int32 aoeLevel = 0;

	//Weapon Logic
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* attackSpawnPoint;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Logic")
		/*
		* The player using the weapon.
		*/
		class APlayerCharacterBase* wieldingPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Logic")
		/*
		* Automatically decreaments by DeltaSeconds each frame.
		*/
		float cooldownTracker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Logic")
		/*
		* The time between shots for foci. Can also be used for the "tick rate" of constantly fired beams. Automatically decreaments by DeltaSeconds each frame.
		*/
		float castDelayTracker;
	#pragma endregion

protected:
	#pragma region Event Implementation
	bool CustomCastCheck_Implementation();

	virtual void Cast_Implementation();
#pragma endregion

	#pragma region UPROPERTIES
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Weapon Logic")
		/*
		* Tracks if the weapon is currently being casted. Used for behavior such as rapid fire or chargable weapons. This is set by the PlayerCharacter casting the focus/spell.
		*/
		bool isCasting = false;
#pragma endregion

public:

	#pragma region UFUNCTIONS
	
	UFUNCTION(BlueprintCallable, Category = "Game Stats")
		/**
		 * Adds a float to a stat's multiplier. The amount the stat is multiplied by is 1+toAdd, so adding 1 (with no other multipliers) will double the base stat.
		 * Stat multipliers stack additively.
		 */
		static void AddStatMultiplier(UPARAM(ref) FMultipliedStat& stat, float toAdd)
	{
		stat.multiplier += toAdd;
		stat.modified = stat.base * stat.multiplier;
	}

	UFUNCTION(BlueprintCallable, Category = "Game Stats")
		static void SetBase(UPARAM(ref) FMultipliedStat& stat, float value)
	{
		stat.base = value;
		stat.modified = stat.base * stat.multiplier;
	}

	UFUNCTION(BlueprintCallable, Category = "Weapon Casting")
		/*Performs the weapon's cooldown/mana/etc. checks, casting the weapon if they pass.*/
		void AttemptCast();

	UFUNCTION(BlueprintCallable, Category = "Weapon Casting")
		/*Sets whether or not the weapon is being casted. If the value changes, the respective Start/Stop Casting events are called.*/
		void SetIsCasting(bool casting);

	AWeaponBase();

	virtual void SetDropState(bool isDropped) override;
	#pragma endregion

	virtual void Tick(float DeltaTime) override;

	void UpdateStatModifier(FMultipliedStat& stat);

protected:
	#pragma region UFUNCTIONS
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon Casting")
		/*
		Starts the casting process for the weapon. By default, this casts the spell, though this could also be used to implement charging a weapon or starting a targeting sequence.
		*/
		void StartCasting();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon Casting")
		/*
		* Stops "casting" the spell. This should typically be called when the player "releases" the button to cast the spell.
		*/
		void StopCasting();
#pragma endregion

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
};

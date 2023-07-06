// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include <string>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "AttackDamageType.h"
#include "WeaponWidget.h"
#include "PlayerHUDWidget.h"
#include "InventoryWidget.h"
#include "ItemBase.h"
#include "WeaponBase.h"
#include "FocusBase.h"
#include "SpellBase.h"
#include "EnemyBase.h"
#include "PlayerCharacterBase.generated.h"

UENUM(BlueprintType)
/*
* Used to describe how items should be sorted in the player's inventory. Favorited items are always before unfavorited items.
* ItemType shows Foci, Spells, and Charms in that order.
*/
enum class EItemSortType : uint8
{
	Name,
	ItemType
};

UCLASS()
class HELLSCAPE_API APlayerCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	//The max number of items player characters can hold. There's no way to buy inventory space or anything silly like that.
	//If this is altered, the inventory blueprint will need the slots changed to the appropriate amount as well.
	const static int inventorySize = 6;

	#pragma region public UPROPERTIES

		//Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		//The transform weapons are parented to when equipped.
		class USceneComponent* WeaponAttachPoint;

		//UI

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UPlayerHUDWidget* HudUI;
		UClass* PlayerHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UInventoryWidget* InventoryUI;
		UClass* InventoryClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Menus")
		bool inCharacterMenu = false;

		//Character Stats

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
		float maxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
		float currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
		float maxMana = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
		float currentMana;
		
	//Inventory

	//Used to know if the inventory is full, and where the last free slot is.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		int itemsInInventory = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		//The foci the player currently have equipped in their inventory, which they can swap between during gameplay.
		TArray<AFocusBase*> fociEquipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		//The focus slot of the focus the player is currently swapped to
		int32 focusSlotWielded = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		//A delay used when the player swaps between foci with axis controls (like the mouse wheel). Helps prevents the player from "double swapping" with the mouse wheel.
		float focusAxisSwapDelay = .1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		//The spells the player currently have equipped. Use the EquipSpell() method to change this property.
		TArray<ASpellBase*> spellsEquipped;
	#pragma endregion

private:
	AItemBase* inventory[inventorySize];

	//Links each damage type to its relavent buildup amount.
	std::map<EDamageType, float> statusEffectBuildup;

	//The distance in Unreal game units that the player can interact with objects from.
	const float interactRange = 650;

	//The player character's turn rate
	float turnRate = 45;

	//The sorting type the player's inventory is currently set to.
	EItemSortType inventorySortType = EItemSortType::Name;

	//The seconds before the player can swap their focus using axis controls (e.g. mouse wheel) again.
	float focusAxisSwapDelayTracker = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Fires the given spell, if its reference isn't null.
	void FireSpell(ASpellBase* spell);

	//Methods bound to player input.
#pragma region Input Methods

		//Character Movement

	void ForwardMovement(float axisValue);

	void LateralMovement(float axisValue);

	void TurnCharacter(float axisValue);

	void LookUp(float axisValue);

	void GamepadTurnCharacter(float axisValue);

	void GamepadLookUp(float axisValue);

		//Combat	

	void FireWieldedFocus();

	void StopFiringWieldedFocus();

	void SwapNextFocus();

	void SwapPrevFocus();

	void AxisSwapFocus(float axisValue);

	void FireSpell1();

	void FireSpell2();

		//Misc

	//Toggles whether or not the character menu is displayed. Because character input must be disabled while in the menu, this function is called by the HellscapePlayerController.
	void ToggleCharacterMenu();

#pragma endregion

public:	

	// Sets default values for this character's properties
	APlayerCharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//A getter for the player's inventory size. Though the inventory size is static and constant for each character, it is easier to use a non-static function for forward declaration
	//(That or I don't know how to make a static function work with a forward declared class...)
	int GetInventorySize();


#pragma region UFUNCTIONS
	UFUNCTION(BlueprintCallable)
		/*Gets the invenotry item at the specified index.*/
		AItemBase* GetInventoryItem(int32 index);

	UFUNCTION(BlueprintCallable)
		/*Returns true if the item was successfully added and false if the inventory was full. Note that this function does NOT update the inventory widget.*/
		bool AddInventoryItem(AItemBase* item);

	UFUNCTION(BlueprintCallable)
		/*Removes the inventory item at the given index. Note that this function does NOT update the inventory widget.*/
		AItemBase* RemoveInventoryItem(int32 index);

	UFUNCTION(BlueprintCallable)
		/*Swaps to the focus in the given focus slot.*/
		void WieldFocusSlot(int32 focusSlot);

	UFUNCTION(Blueprintcallable, BlueprintPure)
		/*Returns the status buildup for a particular damage type.*/
		float GetStatusBuildup(EDamageType damageType);

	UFUNCTION(Blueprintcallable, BlueprintPure)
		/*Sets the status buildup for a given damage type*/
		float SetStatusBuildup(EDamageType damageType, float value);

	UFUNCTION(Blueprintcallable)
		/*Attempts to pick up an item or use an interactable if the player is aimed at one and in range.*/
		void Interact();

	UFUNCTION(BlueprintCallable)
		/*Returns the focus currently wielded by the player*/
		AFocusBase* GetFocusWielded();

	UFUNCTION(Blueprintcallable, Category = "Inventory")
		void PickupItem(AItemBase* item);

	UFUNCTION(Blueprintcallable, Category = "Inventory")
		void DropItem(int32 index);

	UFUNCTION(Blueprintcallable)
		/*
		Updates the inventory widget to display player's current inventory arrangment. 
		This should be called whenever the player's inventory changes while the character menu is open. This includes the inventory being sorted.
		*/
		void UpdateInventoryWidget();

	UFUNCTION(Blueprintcallable, Category = "Inventory")
		void EquipSpell(int32 index, ASpellBase* spell);

	UFUNCTION(Blueprintcallable, Category = "Inventory")
		/*
		Sets the inventory's sort type. As this should be done with the player's inventory open, it will automatically sort and update the player inventory widget.
		*/
		void SetInventorySortType(EItemSortType sortingType);

	UFUNCTION(Blueprintcallable, Category = "Inventory")
		/*
		Sorts the player's inventory by the given sort type. Sorting occurs when the inventory is accessed after new items have been picked up or the player changes the sort type.
		*/
		void SortInventory();

	#pragma region Status Effect Functions

	UFUNCTION(Blueprintcallable, Category = "Status Effects")
		void ApplyStatusEffect(AEnemyBase* target, EStatusEffect statusEffect, AWeaponBase* weaponUsed);

	#pragma endregion

#pragma endregion

private:
	//Sorts alphabetically by name.
	static bool itemNameComparator(AItemBase* item1, AItemBase* item2);

	/*
	Sorts by itemType. The ordering is determined by the values of each member of the EItemType enum, with small values coming first.
	Items of the same type are sorted alphabetically by their name.
	*/
	static bool itemTypeComparator(AItemBase* item1, AItemBase* item2);

	//Let the controller access its player.
	friend class AHellscapePlayerController;
};

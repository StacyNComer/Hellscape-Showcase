// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "WeaponWidget.h"
#include "ItemSlot.generated.h"

/**
 * 
 */
class AItemBase;

UCLASS()
class HELLSCAPE_API UItemSlot : public UWeaponWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EItemType slotType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		/*The item this slot is displaying. Set in blueprints.*/
		AItemBase* itemDisplayed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		/*The index of the player's inventory/equipment this item represents. Set in blueprints.*/
		int32 index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool empty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool favorite;

public:
	//Returns true if the slots are capable of swapping items.
	static bool SlotsCanSwap(UItemSlot* slot1, UItemSlot* slot2)
	{
		//If the slots have the same type or one of the slots is an empty Typeless, return true.
		if (slot1->GetSlotCompatibility() == slot2->GetSlotCompatibility() || (slot1->slotType == EItemType::Typeless && !slot1->itemDisplayed || slot2->slotType == EItemType::Typeless && !slot2->itemDisplayed))
			return true;
		else
			return false;
			
	}

	/*
	Returns the slot type this slot may give it's item to. If the slot is Typeless, the type of item being held is used in place of the slot type, if an item exists.
	Slots with the same compatibility may swap with one another.
	*/
	EItemType GetSlotCompatibility()
	{
		if (slotType == EItemType::Typeless && itemDisplayed)
			return itemDisplayed->itemType;
		else
			return slotType;
	}

	UFUNCTION(BlueprintImplementableEvent)
		/*Toggles the visuals that show whether or not a slot is selected. Slots always begin deselected.*/
		void ToggleSlotSelectEffect();

	virtual void DisplayItem_Implementation(AItemBase* item) override;
};

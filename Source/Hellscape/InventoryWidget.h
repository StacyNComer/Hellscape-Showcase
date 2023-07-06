// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include <vector>
#include "Blueprint/UserWidget.h"
#include "ItemBase.h"
#include "ItemSlot.h"
#include "WeaponWidget.h"
#include "Components/PanelWidget.h"
#include "InventoryWidget.generated.h"

class APlayerCharacterBase;

/**
 * 
 */
UCLASS()
class HELLSCAPE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APlayerCharacterBase* owningPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UItemSlot*> inventorySlots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UItemSlot*> focusSlots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UItemSlot*> spellSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UItemSlot* selectedSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		//This property is meant to be set in Blueprints.
		class UWeaponWidget* weaponDescriptionUI;

public:
	void UpdateDisplay();

	/*Displays the given item to the given slot and handles any changes to the owning player. This function assumes the item belongs in the given slot.*/
	void EquipPlayerFromInventory(UItemSlot* slot, AItemBase* item);

	/*Runs interaction for a slot being "Used". Also handles selected slots being "Used" on other slots.*/
	UFUNCTION(Blueprintcallable)
		void UseItemSlot(UItemSlot* itemSlot);

protected:
	//virtual void NativeOnInitialized() override;

};

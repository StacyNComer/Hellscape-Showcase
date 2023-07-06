// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "PlayerCharacterBase.h"

void UInventoryWidget::UpdateDisplay()
{
	for (int i = 0; i < owningPlayer->GetInventorySize(); i++)
	{
		inventorySlots[i]->DisplayItem(owningPlayer->GetInventoryItem(i));
	}
}

void UInventoryWidget::EquipPlayerFromInventory(UItemSlot* slot, AItemBase* item)
{
	switch (slot->slotType)
	{
		case EItemType::Typeless:
		{
			if (item == nullptr)
				owningPlayer->RemoveInventoryItem(slot->index);
			else
			{
				owningPlayer->AddInventoryItem(item);

				//Remove the inventory slot's original item if items are being swapped.
				if (slot->itemDisplayed != nullptr)
					owningPlayer->RemoveInventoryItem(slot->index);
			}

			break;
		}

		case EItemType::Focus:
		{
			slot->DisplayItem(item);

			//To prevent the cast failing due to a bug being confused with a null item simply being given, the item parameter is used to test if the slot is being emptied.
			AFocusBase* focus = Cast<AFocusBase>(item);
			//The focus which used to be displayed in this slot. It does not need to be type-casted.
			AItemBase* prevFocus = slot->itemDisplayed;

			//If the slot of the focus the player is currently wielding is emptied, try and switch the player to an unemptied slot.
			if (item == nullptr)
			{
				if (prevFocus)
				{
					Cast<AFocusBase>(prevFocus)->SetWielded(false);
				}

				owningPlayer->fociEquipped[slot->index] = focus;

				if (owningPlayer->focusSlotWielded == slot->index)
				{
					for (int i = 0; i < owningPlayer->fociEquipped.Num(); i++)
					{
						if (owningPlayer->fociEquipped[i] != nullptr)
						{
							owningPlayer->WieldFocusSlot(i);

							break;
						}
					}
				}
			}
			//If the player is wielding no focus or was wielding the previously equipped focus, wield the newly equipped focus.
			else if (owningPlayer->GetFocusWielded() == nullptr || owningPlayer->focusSlotWielded == slot->index)
			{
				owningPlayer->fociEquipped[slot->index] = focus;
				owningPlayer->WieldFocusSlot(slot->index);
			}
			else
			{
				owningPlayer->fociEquipped[slot->index] = focus;
			}

			break;
		}

		case EItemType::Spell:
		{
			slot->DisplayItem(item);

			if (item)
			{
				auto spell = Cast<ASpellBase>(item);
			
				spell->wieldingPlayer = owningPlayer;
				owningPlayer->EquipSpell(slot->index, spell);
			}
		
			break;
		}

	/*case EItemType::Charm:
		owningPlayer->fociEquipped[slot->index] = Cast<AFocusBase>(item);
		
		if (GEngine)
							GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("One day, Ill need you!"));
		*/
	}
}

void UInventoryWidget::UseItemSlot(UItemSlot* itemSlot)
{
	if (selectedSlot == nullptr)
	{
		//Selects a slot that is either an empty equipment slot or filled Inventory slot.
		if (!(itemSlot->slotType == EItemType::Typeless && !itemSlot->itemDisplayed))
		{
			itemSlot->ToggleSlotSelectEffect();
			selectedSlot = itemSlot;

			//If the selected slot has an item, view it
			if (selectedSlot->itemDisplayed != nullptr)
			{
				weaponDescriptionUI->DisplayItem(selectedSlot->itemDisplayed);
			}
		}	
	}
	//Deselect the selected slot if it is clicked again or if an empty slot is clicked.
	else if (itemSlot == selectedSlot || (selectedSlot->itemDisplayed == nullptr && itemSlot->itemDisplayed == nullptr))
	{
		selectedSlot->ToggleSlotSelectEffect();
		selectedSlot = nullptr;
	}
	else
	{
		//The selected slot's slot type.
		auto selectedType = selectedSlot->slotType;
		//The slot type of the slot being used.
		auto usedType = itemSlot->slotType;

		//If the slots types are the same, swap whatever is in each slot. If they are different, select the used slot.
		//If either of the slots are an inventory slot, the rules are inverted; If the slots are both Inventory, the used slot is selected and if one slot is different, the slots are swapped.
		if ((selectedType == EItemType::Typeless && usedType == EItemType::Typeless && itemSlot->itemDisplayed != nullptr) || (selectedType != usedType && !(selectedType == EItemType::Typeless || usedType == EItemType::Typeless)))
		//Selecting the used slot.
		{
			selectedSlot->ToggleSlotSelectEffect();

			itemSlot->ToggleSlotSelectEffect();
			selectedSlot = itemSlot;
			weaponDescriptionUI->DisplayItem(selectedSlot->itemDisplayed);
		}
		else if(UItemSlot::SlotsCanSwap(selectedSlot, itemSlot))
		//Swapping items.
		{
			auto itemToSwap = selectedSlot->itemDisplayed;
	
			EquipPlayerFromInventory(selectedSlot, itemSlot->itemDisplayed);
			EquipPlayerFromInventory(itemSlot, itemToSwap);

			//Deselect the current slot
			selectedSlot->ToggleSlotSelectEffect();
			selectedSlot = nullptr;

			owningPlayer->UpdateInventoryWidget();
		}
	}
}

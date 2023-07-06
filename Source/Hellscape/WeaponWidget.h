// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponWidget.generated.h"

class AItemBase;

UCLASS()
/**
 * A base class that can be used for any widget that needs to display a weapon.
 */
class HELLSCAPE_API UWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/*
		* Updates the widget to display/respond to a particular weapon. Meant to be implemented in blueprints.
		*/
		void DisplayItem(AItemBase* item);

	virtual void DisplayItem_Implementation(AItemBase* item);
};

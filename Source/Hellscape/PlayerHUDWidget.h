// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PlayerHUDWidget.generated.h"

class APlayerCharacterBase;
class ASpellBase;

/**
 * A class for the player's HUD Widget. Any interactions that must be handled from C++ are set up here. The rest of the HUD is scripted using a derived blueprint.
 */
UCLASS()
class HELLSCAPE_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	ASpellBase* spellEquipped[2];

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerCharacterBase* owningPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//The UI components displaying the player's spells. The references must be set in blueprints.
	TArray<UProgressBar*> spellUI;

	void SetEquippedSpell(int32 index, ASpellBase* spell);

	UFUNCTION(BlueprintCallable)
	float GetSpellCooldownPercent(int32 index);	
};

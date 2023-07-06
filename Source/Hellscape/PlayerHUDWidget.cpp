// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "SpellBase.h"
#include "PlayerCharacterBase.h"
#include "Components/ProgressBar.h"
#include "Brushes/SlateImageBrush.h"
#include "Math/Vector2D.h"

void UPlayerHUDWidget::SetEquippedSpell(int32 index, ASpellBase* spell)
{
	spellUI[index]->WidgetStyle.BackgroundImage.SetResourceObject(spell->itemImage);
}

float UPlayerHUDWidget::GetSpellCooldownPercent(int32 index)
{
	auto spell = owningPlayer->spellsEquipped[index];

	if (spell)
		return spell->cooldownTracker / spell->cooldown;
	else
		return 0;
}
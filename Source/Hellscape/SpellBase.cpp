// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellBase.h"

ASpellBase::ASpellBase()
{
	itemType = EItemType::Spell;
	itemTypeName = NSLOCTEXT("ItemTypeName", "SpellType", "Spell");
}